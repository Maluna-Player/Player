/*************************************
 * @file    PlayerSocket.cpp
 * @date    16/06/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerSocket.
 *************************************
*/

#include "PlayerSocket.h"
#include "../Exceptions/LibException.h"
#include "../Network/RemoteSong.h"


PlayerSocket::PlayerSocket()
    : m_Connected(false), mp_Server(0), mp_Socket(0), messageSize(0),
      nbRecievedSongs(0), nbSongsToRecieve(-1)
{
    mp_RecievedSongs = new QTreeWidgetItem;
}

// ==============================
// ==============================

PlayerSocket::~PlayerSocket()
{
    if (mp_Server)
        mp_Server->deleteLater();

    if (mp_Socket)
        mp_Socket->deleteLater();

    delete mp_RecievedSongs;
}

// ==============================
// ==============================

void PlayerSocket::listen(QHostAddress address)
{
    mp_Server = new QTcpServer(this);

    if (!mp_Server->listen(address, 1200))
        throw LibException("PlayerSocket::listen", "QTcpServer::listen", mp_Server->errorString().toStdString().c_str());

    connect(mp_Server, SIGNAL(newConnection()), this, SLOT(clientConnexion()));
}

// ==============================
// ==============================

void PlayerSocket::clientConnexion()
{
    mp_Socket = mp_Server->nextPendingConnection();

    connect(mp_Socket, SIGNAL(disconnected()), this, SLOT(clientDisconnection()));

    startConnection();

    mp_Server->close();
}

// ==============================
// ==============================

void PlayerSocket::startConnection()
{
    m_Connected = true;

    emit(connected());
}

// ==============================
// ==============================

void PlayerSocket::disconnection()
{
    mp_Socket->close();

    emit(endedConnection());
}

// ==============================
// ==============================

void PlayerSocket::clientDisconnection()
{
    emit(endedConnection());
}

// ==============================
// ==============================

void PlayerSocket::connectToHost(const QString& address)
{
    mp_Socket = new QTcpSocket(this);
    mp_Socket->abort();

    QString host = (address.isEmpty()) ? "localhost" : address;
    mp_Socket->connectToHost(host, 1200);

    connect(mp_Socket, SIGNAL(connected()), this, SLOT(startConnection()));
    connect(mp_Socket, SIGNAL(disconnected()), this, SLOT(disconnection()));
    connect(mp_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
}

// ==============================
// ==============================

void PlayerSocket::error()
{
    qDebug() << mp_Socket->errorString();
 }

// ==============================
// ==============================

void PlayerSocket::send(const QList<QTreeWidgetItem*>& songs, int nbSongs)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    // Envoi du nombre de musiques
    out << static_cast<quint8>(nbSongs);

    mp_Socket->write(packet);

    mp_SendingThread = new SendingThread(mp_Socket, songs);
    mp_SendingThread->start();

    mp_SendingThread->wait();
    delete mp_SendingThread;

    connect(mp_Socket, SIGNAL(readyRead()), this, SLOT(readRemoteSongList()));
}

// ==============================
// ==============================

QTreeWidgetItem* PlayerSocket::getItem(int num, QTreeWidgetItem *parent) const
{
    if (!parent)
        parent = mp_RecievedSongs;

    for (int i = 0; i < parent->childCount(); i++)
    {
        bool isInt;
        int itemNum = parent->child(i)->data(0, Qt::UserRole).toInt(&isInt);

        if (isInt && itemNum == num)
            return parent->child(i);

        getItem(num, parent->child(i));
    }

    return 0;
}

// ==============================
// ==============================

void PlayerSocket::readRemoteSongList()
{
    while (mp_Socket->bytesAvailable() > 0)
    {
        QDataStream in(mp_Socket);

        if (messageSize == 0)
        {
            if (nbSongsToRecieve == -1)
            {
                if (mp_Socket->bytesAvailable() < sizeof(quint8))
                    return;
                else
                {
                    quint8 nbSongs;
                    in >> nbSongs;
                    nbSongsToRecieve = nbSongs;
                }
            }

            if (mp_Socket->bytesAvailable() < sizeof(quint16))
                 return;

            in >> messageSize;
        }

        if (mp_Socket->bytesAvailable() < messageSize)
            return;

        quint16 num;
        quint16 parent;
        QString fileName;
        quint8 itemType;
        quint32 songLength;

        in >> num >> parent >> fileName >> itemType;

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, fileName);

        if (itemType == 0)
            item->setData(0, Qt::UserRole, num);
        else
        {
            in >> songLength;

            Song *song = new RemoteSong(fileName, nbRecievedSongs, songLength);
            item->setData(0, Qt::UserRole, reinterpret_cast<quintptr>(song));

            nbRecievedSongs++;
        }

        if (!parent)
            mp_RecievedSongs->addChild(item);
        else
            getItem(parent)->addChild(item);

        messageSize = 0;
    }

    if (nbRecievedSongs == nbSongsToRecieve)
        emit(songsRecieved(mp_RecievedSongs->takeChildren()));
}
