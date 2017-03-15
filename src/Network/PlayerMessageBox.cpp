/*************************************
 * @file    PlayerMessageBox.cpp
 * @date    19/06/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerMessageBox.
 *************************************
*/

#include "PlayerMessageBox.h"
#include <QDataStream>
#include <QEventLoop>


namespace network {


PlayerMessageBox::PlayerMessageBox(QTcpSocket *socket) : mp_Socket(socket), m_MessageSize(0), m_IsSending(false)
{
    connect(mp_Socket, &QTcpSocket::readyRead, this, &PlayerMessageBox::receiveMessages);
    connect(this, &PlayerMessageBox::newMessageToSend, this, &PlayerMessageBox::sendMessages);
}

// ==============================
// ==============================

QByteArray PlayerMessageBox::getNextMessageToSend()
{
    QByteArray message;

    m_SendingMutex.lock();

    if (!m_MessagesToSend.isEmpty())
        message = m_MessagesToSend.takeAt(0);

    m_SendingMutex.unlock();

    return message;
}

// ==============================
// ==============================

QByteArray PlayerMessageBox::getNextMessage()
{
    QByteArray message;

    if (m_MessageGettingMutex.tryLock())
    {
        m_ReceivingMutex.lock();

        if (!m_ReceivedMessages.isEmpty())
            message = m_ReceivedMessages.takeAt(0);

        m_ReceivingMutex.unlock();
        m_MessageGettingMutex.unlock();
    }

    return message;
}

// ==============================
// ==============================

QByteArray PlayerMessageBox::waitNextMessage()
{
    QByteArray message;

    m_MessageGettingMutex.lock();
    m_ReceivingMutex.lock();

    if (!m_ReceivedMessages.isEmpty())
    {
        message = m_ReceivedMessages.takeAt(0);

        m_ReceivingMutex.unlock();
        m_MessageGettingMutex.unlock();

        return message;
    }

    m_ReceivingMutex.unlock();

    QEventLoop pause;
    connect(this, &PlayerMessageBox::messageReceived, &pause, &QEventLoop::quit);
    pause.exec();

    m_ReceivingMutex.lock();

    if (!m_ReceivedMessages.isEmpty())
        message = m_ReceivedMessages.takeAt(0);

    m_ReceivingMutex.unlock();
    m_MessageGettingMutex.unlock();

    return message;
}

// ==============================
// ==============================

void PlayerMessageBox::add(const QByteArray& message)
{
    m_SendingMutex.lock();

    m_MessagesToSend.append(message);
    if (!m_IsSending)
        emit newMessageToSend();

    m_SendingMutex.unlock();
}

// ==============================
// ==============================

void PlayerMessageBox::add(Sendable *objectToSend)
{
    add(objectToSend->toPacket());
}

// ==============================
// ==============================

void PlayerMessageBox::sendMessages()
{
    m_SendingMutex.lock();
    m_IsSending = true;
    m_SendingMutex.unlock();

    QByteArray message;

    while (!((message = getNextMessageToSend()).isEmpty()))
    {
        QByteArray messageSize;
        QDataStream out(&messageSize, QIODevice::WriteOnly);

        out << static_cast<MessageSize_t>(message.size());

        // Ajout de la taille du message dans l'en-tête
        message.prepend(messageSize);

        mp_Socket->write(message);

        m_SendingMutex.lock();

        if (m_MessagesToSend.isEmpty())
            m_IsSending = false;

        m_SendingMutex.unlock();
    }

    m_SendingMutex.lock();
    m_IsSending = false;
    m_SendingMutex.unlock();
}

// ==============================
// ==============================

void PlayerMessageBox::receiveMessages()
{
    while (mp_Socket->bytesAvailable() > 0)
    {
        QDataStream in(mp_Socket);

        if (m_MessageSize == 0)
        {
            if (mp_Socket->bytesAvailable() < sizeof(MessageSize_t))
                 return;

            in >> m_MessageSize;
        }

        if (mp_Socket->bytesAvailable() < m_MessageSize)
            return;

        QByteArray message(m_MessageSize, 0);
        mp_Socket->read(message.data(), m_MessageSize);

        m_ReceivingMutex.lock();
        m_ReceivedMessages.append(message);
        m_ReceivingMutex.unlock();

        emit messageReceived();
        m_MessageSize = 0;
    }
}


} // network
