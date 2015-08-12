/*************************************
 * @file    PlayerMessage.cpp
 * @date    19/06/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerMessage.
 *************************************
*/

#include "PlayerMessage.h"
#include <QDataStream>
#include <QEventLoop>


PlayerMessage::PlayerMessage(QTcpSocket *socket) : mp_Socket(socket), m_MessageSize(0), m_IsSending(false)
{
    connect(this, SIGNAL(newMessageToSend()), this, SLOT(sendMessages()));
}

// ==============================
// ==============================

PlayerMessage::~PlayerMessage()
{

}

// ==============================
// ==============================

QByteArray PlayerMessage::getNextMessage()
{
    QByteArray message;

    if (m_MessageGettingMutex.tryLock())
    {
        m_MessagesMutex.lock();

        if (!m_Messages.isEmpty())
            message = m_Messages.takeAt(0);

        m_MessagesMutex.unlock();
        m_MessageGettingMutex.unlock();
    }

    return message;
}

// ==============================
// ==============================

QByteArray PlayerMessage::waitNextMessage()
{
    QByteArray message;

    m_MessageGettingMutex.lock();
    m_MessagesMutex.lock();

    if (!m_Messages.isEmpty())
    {
        message = m_Messages.takeAt(0);

        m_MessagesMutex.unlock();
        m_MessageGettingMutex.unlock();

        return message;
    }

    m_MessagesMutex.unlock();

    QEventLoop pause;
    connect(this, SIGNAL(messageReceived()), &pause, SLOT(quit()));
    pause.exec();

    m_MessagesMutex.lock();

    if (!m_Messages.isEmpty())
        message = m_Messages.takeAt(0);

    m_MessagesMutex.unlock();
    m_MessageGettingMutex.unlock();

    return message;
}

// ==============================
// ==============================

void PlayerMessage::add(const QByteArray& message)
{
    m_MessagesMutex.lock();

    m_Messages.append(message);
    if (!m_IsSending)
        emit newMessageToSend();

    m_MessagesMutex.unlock();
}

// ==============================
// ==============================

void PlayerMessage::add(Sendable *objectToSend)
{
    add(objectToSend->toPacket());
}

// ==============================
// ==============================

void PlayerMessage::sendMessages()
{
    m_MessagesMutex.lock();
    m_IsSending = true;
    m_MessagesMutex.unlock();

    QByteArray message;

    while (!((message = getNextMessage()).isEmpty()))
    {
        QByteArray messageSize;
        QDataStream out(&messageSize, QIODevice::WriteOnly);

        out << static_cast<MessageSize_t>(message.size());

        // Ajout de la taille du message dans l'en-tête
        message.prepend(messageSize);

        mp_Socket->write(message);

        m_MessagesMutex.lock();

        if (m_Messages.isEmpty())
            m_IsSending = false;

        m_MessagesMutex.unlock();
    }

    m_MessagesMutex.lock();
    m_IsSending = false;
    m_MessagesMutex.unlock();
}

// ==============================
// ==============================

void PlayerMessage::receive()
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

        m_MessagesMutex.lock();
        m_Messages.append(message);
        m_MessagesMutex.unlock();

        emit messageReceived();
        m_MessageSize = 0;
    }
}
