/*************************************
 * @file    PlayerSocket.cpp
 * @date    16/06/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerSocket.
 *************************************
*/

#include "PlayerSocket.h"
#include "RemoteSong.h"
#include "../Exceptions/LibException.h"
#include "../Exceptions/ArrayAccessException.h"


namespace network {


PlayerSocket::PlayerSocket(audio::Player *player)
    : mp_Player(player), m_Connected(false), mp_Server(nullptr), mp_Socket(nullptr), m_NbSentListItems(0), m_NbReceivedSongs(0),
      mp_SendMessage(nullptr), mp_ReceiveMessage(nullptr), mp_SocketThread(nullptr)
{
    m_CallbackSettings.openCallback = openCallback;
    m_CallbackSettings.closeCallback = closeCallback;
    m_CallbackSettings.readCallback = readCallback;
    m_CallbackSettings.seekCallback = seekCallback;
    m_CallbackSettings.userdata = this;
}

// ==============================
// ==============================

PlayerSocket::~PlayerSocket()
{
    if (mp_Server)
        mp_Server->deleteLater();

    if (mp_Socket)
        mp_Socket->deleteLater();

    if (mp_SocketThread)
        mp_SocketThread->deleteLater();
}

// ==============================
// ==============================

audio::SoundSettings& PlayerSocket::getCallbackSettings()
{
    return m_CallbackSettings;
}

// ==============================
// ==============================

quint32 PlayerSocket::getSongDataReceived() const
{
    return m_SongDataReceived;
}

// ==============================
// ==============================

quint32 PlayerSocket::getTotalCurrentSongData() const
{
    return m_TotalCurrentSongData;
}

// ==============================
// ==============================

bool PlayerSocket::isConnected() const
{
    return m_Connected;
}

// ==============================
// ==============================

void PlayerSocket::listen(QHostAddress address)
{
    mp_Server = new QTcpServer(this);

    if (!mp_Server->listen(address, 1200))
        throw exceptions::LibException("PlayerSocket::listen", "QTcpServer::listen", mp_Server->errorString().toStdString().c_str());

    connect(mp_Server, SIGNAL(newConnection()), this, SLOT(clientConnexion()));
}

// ==============================
// ==============================

void PlayerSocket::clientConnexion()
{
    mp_Socket = mp_Server->nextPendingConnection();

    connect(mp_Socket, SIGNAL(disconnected()), this, SLOT(disconnection()));

    startConnection();

    mp_Server->deleteLater();
    mp_Server = nullptr;
}

// ==============================
// ==============================

void PlayerSocket::startConnection()
{
    mp_SocketThread = new QThread(this);
    mp_SocketThread->start();

    mp_Socket->setParent(nullptr);
    mp_Socket->moveToThread(mp_SocketThread);

    mp_SendMessage = std::make_unique<PlayerMessage>(mp_Socket);
    mp_ReceiveMessage = std::make_unique<PlayerMessage>(mp_Socket);

    connect(mp_Socket, SIGNAL(readyRead()), mp_ReceiveMessage.get(), SLOT(receive()));

    mp_SendMessage->moveToThread(mp_SocketThread);
    mp_ReceiveMessage->moveToThread(mp_SocketThread);

    emit connected();
}

// ==============================
// ==============================

void PlayerSocket::disconnection()
{
    mp_Socket->deleteLater();
    mp_Socket = nullptr;

    if (mp_SocketThread)
    {
        mp_SocketThread->quit();
        mp_SocketThread->wait();
    }

    m_Connected = false;

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
    connect(mp_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()), Qt::DirectConnection);
}

// ==============================
// ==============================

void PlayerSocket::error()
{
    qDebug() << mp_Socket->errorString();
}

// ==============================
// ==============================

void PlayerSocket::sendSongs(gui::SongTreeRoot *item)
{
    if (!item->isRoot())
        mp_SendMessage->add(item);

    for (int i = 0; i < item->childCount(); i++)
        sendSongs(static_cast<gui::SongListItem*>(item->child(i)));
}

// ==============================
// ==============================

gui::SongListItem* PlayerSocket::getItem(int num, gui::SongTreeRoot *parent) const
{
    for (int i = 0; i < parent->childCount(); i++)
    {
        gui::SongListItem *child = static_cast<gui::SongListItem*>(parent->child(i));

        bool isInt;
        int itemNum = child->data(0, Qt::UserRole).toInt(&isInt);

        if (isInt && itemNum == num)
            return child;

        gui::SongListItem *item = getItem(num, child);
        if (item)
            return item;
    }

    return nullptr;
}

// ==============================
// ==============================

gui::SongTreeRoot* PlayerSocket::readRemoteSongList()
{
    gui::SongListItem *receivedSongs = new gui::SongListItem(gui::SongListItem::ElementType::ROOT);
    QByteArray message = mp_ReceiveMessage->waitNextMessage();

    QDataStream in(&message, QIODevice::ReadOnly);
    quint8 nbSongs;

    in >> nbSongs;

    while (m_NbReceivedSongs < nbSongs)
    {
        message = mp_ReceiveMessage->waitNextMessage();
        in.device()->seek(0);

        quint16 num;
        quint16 parentNum;
        QString fileName;
        quint8 itemType;

        in >> num >> parentNum >> fileName >> itemType;

        gui::SongListItem::ElementType type = (itemType == 0) ? gui::SongListItem::ElementType::DIRECTORY : gui::SongListItem::ElementType::SONG;
        gui::SongListItem *item = new gui::SongListItem(type, getItem(parentNum, receivedSongs), fileName);

        if (!item->isSong())
            item->setData(0, Qt::UserRole, num);
        else
        {
            quint16 songNum;
            quint32 songLength;
            QString artist;

            in >> songNum;
            in >> songLength;
            in >> artist;

            std::shared_ptr<network::RemoteSong> song = mp_Player->createRemoteSong(fileName, songNum, songLength, artist, &getCallbackSettings());
            item->setAttachedSong(song);

            m_NbReceivedSongs++;
        }

        if (!item->parent())
            receivedSongs->addChild(item);
    }

    return receivedSongs;
}

// ==============================
// ==============================

gui::SongTreeRoot* PlayerSocket::exchangeSongList(gui::SongTreeRoot *songs)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    // Envoi du nombre de musiques
    out << static_cast<quint8>(mp_Player->songsCount());

    mp_SendMessage->add(packet);

    sendSongs(songs);

    gui::SongTreeRoot *receivedSongs = readRemoteSongList();
    m_Connected = true;

    return receivedSongs;
}

// ==============================
// ==============================

std::shared_ptr<commands::Command> PlayerSocket::buildCommand(QByteArray message) const
{
    QDataStream in(&message, QIODevice::ReadOnly);
    std::shared_ptr<commands::Command> command { nullptr };

    quint8 messageType;
    quint8 commandType;
    quint16 songNum;

    in >> messageType >> commandType >> songNum;

    if (messageType == 'C')
    {
        switch (commandType)
        {
            case 'o':
                command = std::make_shared<commands::OpenCommandRequest>(songNum);
                break;

            case 'c':
                command = std::make_shared<commands::CloseCommandRequest>(songNum);
                break;

            case 'r':
            {
                quint32 bytesToRead;
                in >> bytesToRead;

                command = std::make_shared<commands::ReadCommandRequest>(songNum, bytesToRead);
                break;
            }
            case 's':
            {
                quint32 pos;
                in >> pos;

                command = std::make_shared<commands::SeekCommandRequest>(songNum, pos);
                break;
            }

            default:
                break;
        }
    }
    else
    {
        quint8 result;
        in >> result;

        switch (commandType)
        {
            case 'o':
            {
                quint32 fileSize;
                in >> fileSize;

                command = std::make_shared<commands::OpenCommandReply>(songNum, static_cast<FMOD_RESULT>(result), fileSize);
                break;
            }
            case 'c':
                command = std::make_shared<commands::CloseCommandReply>(songNum, static_cast<FMOD_RESULT>(result));
                break;

            case 'r':
            {
                quint32 readBytes;
                in >> readBytes;

                char *buffer = new char[readBytes];
                in.readRawData(buffer, readBytes);

                command = std::make_shared<commands::ReadCommandReply>(songNum, static_cast<FMOD_RESULT>(result), buffer, readBytes);
                break;
            }
            case 's':
                command = std::make_shared<commands::SeekCommandReply>(songNum, static_cast<FMOD_RESULT>(result));
                break;

            default:
                break;
        }
    }

    return command;
}

// ==============================
// ==============================

std::shared_ptr<commands::CommandRequest> PlayerSocket::getCommandRequest()
{
    if (!mp_ReceivedRequests.isEmpty())
        return mp_ReceivedRequests.takeAt(0);
    else
    {
        std::shared_ptr<commands::Command> command { nullptr };

        do
        {
            QByteArray message = mp_ReceiveMessage->getNextMessage();
            if (message.isEmpty())
                return nullptr;

            command = buildCommand(message);
            if (command && command->isReply())
                mp_ReceivedReplies.append(std::static_pointer_cast<commands::CommandReply>(command));

        } while (!(command && command->isRequest()));

        return std::static_pointer_cast<commands::CommandRequest>(command);
    }
}

// ==============================
// ==============================

std::shared_ptr<commands::CommandReply> PlayerSocket::getCommandReply()
{
    if (!mp_ReceivedReplies.isEmpty())
        return mp_ReceivedReplies.takeAt(0);
    else
    {
        std::shared_ptr<commands::Command> command { nullptr };

        do
        {
            command = buildCommand(mp_ReceiveMessage->waitNextMessage());
            if (command && command->isRequest())
                mp_ReceivedRequests.append(std::static_pointer_cast<commands::CommandRequest>(command));

        } while (!(command && command->isReply()));

        return std::static_pointer_cast<commands::CommandReply>(command);
    }
}

// ==============================
// ==============================

FMOD_RESULT PlayerSocket::openRemoteFile(const char *fileName, unsigned int *filesize, void **handle)
{
    if (fileName)
    {
        int *songNum = new int(atoi(fileName));
        if (*songNum >= m_NbReceivedSongs)
            throw exceptions::ArrayAccessException("PlayerSocket::openRemoteFile", m_NbReceivedSongs, *songNum);

        commands::OpenCommandRequest request(*songNum);
        mp_SendMessage->add(&request);

        std::shared_ptr<commands::OpenCommandReply> reply = std::static_pointer_cast<commands::OpenCommandReply>(getCommandReply());

        if (reply)
        {
            FMOD_RESULT result = reply->getResult();

            *filesize = static_cast<unsigned int>(reply->getFileSize());
            *handle = songNum;

            m_TotalCurrentSongData = *filesize;
            m_SongDataReceived = 0;

            return result;
        }
    }

    return FMOD_OK;
}

// ==============================
// ==============================

FMOD_RESULT PlayerSocket::closeRemoteFile(void *handle)
{
    if (!handle)
        return FMOD_ERR_INVALID_PARAM;

    int *songNum = static_cast<int*>(handle);

    commands::CloseCommandRequest request(*songNum);
    mp_SendMessage->add(&request);

    std::shared_ptr<commands::CloseCommandReply> reply = std::static_pointer_cast<commands::CloseCommandReply>(getCommandReply());
    FMOD_RESULT result = FMOD_OK;

    if (reply)
        result = reply->getResult();

    delete songNum;

    return result;
}

// ==============================
// ==============================

FMOD_RESULT PlayerSocket::readRemoteFile(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread)
{
    if (!handle)
        return FMOD_ERR_INVALID_PARAM;

    if (bytesread)
    {
        int *songNum = static_cast<int*>(handle);

        commands::ReadCommandRequest request(*songNum, sizebytes);
        mp_SendMessage->add(&request);

        std::shared_ptr<commands::ReadCommandReply> reply = std::static_pointer_cast<commands::ReadCommandReply>(getCommandReply());
        if (reply)
        {
            FMOD_RESULT result = reply->getResult();

            *bytesread = reply->getReadBytes();
            memcpy(buffer, reply->getBuffer(), *bytesread);

            m_SongDataReceived += *bytesread;

            if (*bytesread < sizebytes)
                return FMOD_ERR_FILE_EOF;

            return result;
        }
    }

    return FMOD_OK;
}

// ==============================
// ==============================

FMOD_RESULT PlayerSocket::seekRemoteFile(void *handle, unsigned int pos)
{
    if (!handle)
        return FMOD_ERR_INVALID_PARAM;

    int *songNum = static_cast<int*>(handle);

    commands::SeekCommandRequest request(*songNum, pos);
    mp_SendMessage->add(&request);

    std::shared_ptr<commands::SeekCommandReply> reply = std::static_pointer_cast<commands::SeekCommandReply>(getCommandReply());
    FMOD_RESULT result = FMOD_OK;

    if (reply)
    {
        result = reply->getResult();
        m_SongDataReceived = 0;
    }

    return result;
}

// ==============================
// ==============================

void PlayerSocket::processCommands()
{
    std::shared_ptr<commands::CommandRequest> request = getCommandRequest();

    if (request)
        emit commandReceived(request);
}

// ==============================
// ==============================

void PlayerSocket::sendCommandReply(std::shared_ptr<commands::CommandReply> reply)
{
    mp_SendMessage->add(reply.get());
}

// ==============================
// ==============================

FMOD_RESULT openCallback(const char *fileName, unsigned int *filesize, void **handle, void *userdata)
{
    return static_cast<PlayerSocket*>(userdata)->openRemoteFile(fileName, filesize, handle);
}

// ==============================
// ==============================

FMOD_RESULT closeCallback(void *handle, void *userdata)
{
    return static_cast<PlayerSocket*>(userdata)->closeRemoteFile(handle);
}

// ==============================
// ==============================

FMOD_RESULT readCallback(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata)
{
    return static_cast<PlayerSocket*>(userdata)->readRemoteFile(handle, buffer, sizebytes, bytesread);
}

// ==============================
// ==============================

FMOD_RESULT seekCallback(void *handle, unsigned int pos, void *userdata)
{
    return static_cast<PlayerSocket*>(userdata)->seekRemoteFile(handle, pos);
}


} // network
