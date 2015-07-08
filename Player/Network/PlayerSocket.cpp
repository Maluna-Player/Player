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
#include "SongListItem.h"


PlayerSocket::PlayerSocket()
    : m_Connected(false), mp_Server(0), mp_Socket(0), m_NbSentListItems(0)
{

}

// ==============================
// ==============================

PlayerSocket::~PlayerSocket()
{
    if (mp_Server)
        mp_Server->deleteLater();

    if (mp_Socket)
        mp_Socket->deleteLater();

    for (int i = 0; i < mp_RemoteSongs.size(); i++)
        delete mp_RemoteSongs[i];
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

    mp_SocketThread = new QThread(this);
    mp_SocketThread->start();

    mp_Socket->setParent(0);
    mp_Socket->moveToThread(mp_SocketThread);

    mp_SendMessage = new PlayerMessage(mp_Socket);
    mp_ReceiveMessage = new PlayerMessage(mp_Socket);

    connect(mp_Socket, SIGNAL(readyRead()), mp_ReceiveMessage, SLOT(receive()));

    mp_SendMessage->moveToThread(mp_SocketThread);
    mp_ReceiveMessage->moveToThread(mp_SocketThread);

    emit connected();
}

// ==============================
// ==============================

void PlayerSocket::disconnection()
{
    mp_Socket->close();

    mp_SocketThread->quit();

    delete mp_SendMessage;
    delete mp_ReceiveMessage;

    emit disconnected();
}

// ==============================
// ==============================

void PlayerSocket::clientDisconnection()
{
    emit disconnected();
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

void PlayerSocket::sendSongs(QTreeWidgetItem *item, int parent)
{
    int num = m_NbSentListItems + 1;

    SongListItem *songListItem = 0;

    if (item->childCount() > 0)
        songListItem = new SongListItem(num, item->text(0), parent);
    else
    {
        Song *song = reinterpret_cast<Song*>(item->data(0, Qt::UserRole).value<quintptr>());
        if (song)
            songListItem = new SongListItem(num, song->getLength(), item->text(0), parent);
    }

    if (songListItem)
    {
        mp_SendMessage->add(songListItem);
        delete songListItem;
    }

    m_NbSentListItems++;

    for (int i = 0; i < item->childCount(); i++)
        sendSongs(item->child(i), num);
}

// ==============================
// ==============================

QTreeWidgetItem* PlayerSocket::getItem(int num, QTreeWidgetItem *parent) const
{
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

QList<QTreeWidgetItem*> PlayerSocket::readRemoteSongList()
{
    QTreeWidgetItem *receivedSongs = new QTreeWidgetItem;
    QByteArray message = mp_ReceiveMessage->waitNextMessage();

    QDataStream in(&message, QIODevice::ReadOnly);
    quint8 nbSongs;

    in >> nbSongs;

    while (mp_RemoteSongs.size() < nbSongs)
    {
        message = mp_ReceiveMessage->waitNextMessage();
        in.device()->seek(0);

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

            RemoteSong *song = new RemoteSong(fileName, mp_RemoteSongs.size(), songLength);
            item->setData(0, Qt::UserRole, reinterpret_cast<quintptr>(song));

            mp_RemoteSongs.append(song);
        }

        if (!parent)
            receivedSongs->addChild(item);
        else
            getItem(parent, receivedSongs)->addChild(item);
    }

    QList<QTreeWidgetItem*> children = receivedSongs->takeChildren();
    delete receivedSongs;

    return children;
}

// ==============================
// ==============================

QList<QTreeWidgetItem*> PlayerSocket::exchangeSongList(const QList<QTreeWidgetItem*>& songs, int nbSongs)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    // Envoi du nombre de musiques
    out << static_cast<quint8>(nbSongs);

    mp_SendMessage->add(packet);

    for (int i = 0; i < songs.size(); i++)
        sendSongs(songs.at(i), 0);

    return readRemoteSongList();
}
