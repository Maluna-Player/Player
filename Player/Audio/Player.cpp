/*************************************
 * @file    Player.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Player.
 *************************************
*/

#include "Player.h"
#include "../Exceptions/ArrayAccessException.h"
#include "../Exceptions/LibException.h"
#include "../Exceptions/FileLoadingException.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>


Player::Player()
    : m_CurrentSong(0), m_Playlist(true), m_Loop(false),
      m_Pause(false), m_Stop(true), m_Mute(false),
      m_VolumeState(NB_VOLUME_STATES - 1)
{

}

// ==============================
// ==============================

Player::~Player()
{
    for (int i = 0; i < mp_Songs.size(); i++)
        delete mp_Songs[i];

    mp_Songs.clear();
}

// ==============================
// ==============================

Song& Player::getCurrentSong()
{
    if (m_CurrentSong >= mp_Songs.size())
        throw ArrayAccessException("Player::getCurrentSong", mp_Songs.size(), m_CurrentSong);

    return *(mp_Songs[m_CurrentSong]);
}

// ==============================
// ==============================

int Player::songsCount() const
{
    return mp_Songs.size();
}

// ==============================
// ==============================

void Player::play()
{
    if (mp_Songs.size() > 0)
    {
        if (m_Stop)
        {
            m_Stop = false;
            getCurrentSong().play();
        }
        else if (m_Pause)
        {
            m_Pause = false;
            getCurrentSong().pause(false);
        }
    }
}

// ==============================
// ==============================

void Player::stop()
{
    if (!isStopped())
    {
        m_Pause = false;
        m_Stop = true;

        if (mp_Songs.size() > 0)
            getCurrentSong().stop();
    }
}

// ==============================
// ==============================

void Player::pause()
{
    m_Pause = true;

    if (mp_Songs.size() > 0)
        getCurrentSong().pause(true);
}

// ==============================
// ==============================

void Player::mute(bool mute)
{
    m_Mute = mute;
    FmodManager::getInstance()->setMute(mute);
}

// ==============================
// ==============================

bool Player::isPlaying() const
{
    return (!m_Pause && !m_Stop);
}

// ==============================
// ==============================

bool Player::isStopped() const
{
    return m_Stop;
}

// ==============================
// ==============================

bool Player::isPaused() const
{
    return m_Pause;
}

// ==============================
// ==============================

bool Player::isMuted() const
{
    return m_Mute;
}

// ==============================
// ==============================

bool Player::isLoop() const
{
    return m_Loop;
}

// ==============================
// ==============================

void Player::setLoop(bool loop)
{
    m_Loop = loop;
}

// ==============================
// ==============================

int Player::first() const
{
    if (mp_Songs.size() > 0)
        return FIRST_SONG;
    else
        return UNDEFINED_SONG;
}

// ==============================
// ==============================

int Player::prev() const
{
    if (m_CurrentSong > FIRST_SONG)
        return (m_CurrentSong - 1);
    else if (m_Loop && mp_Songs.size() > 0)
        return LAST_SONG;
    else
        return UNDEFINED_SONG;
}

// ==============================
// ==============================

int Player::next() const
{
    if (m_CurrentSong < LAST_SONG)
        return (m_CurrentSong + 1);
    else if (m_Loop && mp_Songs.size() > 0)
        return FIRST_SONG;
    else
        return UNDEFINED_SONG;
}

// ==============================
// ==============================

int Player::getVolumeState() const
{
    return m_VolumeState;
}

// ==============================
// ==============================

void Player::setVolume(int volumeState)
{
    m_VolumeState = volumeState;

    float volume = static_cast<float>(volumeState) / (NB_VOLUME_STATES - 1);
    FmodManager::getInstance()->setVolume(volume);
}

// ==============================
// ==============================

void Player::clearSongs()
{
    for (int i = 0; i < mp_Songs.size(); i++)
        delete mp_Songs[i];

    mp_Songs.clear();
}

// ==============================
// ==============================

void Player::addNewSong(const QString& filePath, SongListItem *parentDir)
{
    QFileInfo fileInfo(filePath);
    SongListItem *item = new SongListItem(SongListItem::SONG, parentDir, fileInfo.completeBaseName());

    try
    {
        Song *song = new Song(filePath, mp_Songs.size());

        mp_Songs.append(song);
        item->setData(0, Qt::UserRole, reinterpret_cast<quintptr>(song));
    }
    catch (FmodManager::StreamError_t error)
    {
        if (error == FmodManager::FILE_ERROR)
            qWarning() << "Error loading" << filePath;
        else if (error == FmodManager::FORMAT_ERROR)
            qWarning() << "Unsupported format for" << filePath;

        delete item;
    }
}

// ==============================
// ==============================

SongTreeRoot* Player::loadSongs(const QString& dirPath, SongTreeRoot *parentDir)
{
    if (!parentDir)
        parentDir = new SongTreeRoot(SongListItem::ROOT);

    QDir dir(dirPath);
    if (!dir.exists())
        throw FileLoadingException("Player::loadSongs", dirPath.toStdString());

    QFileInfoList files = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (int i = 0; i < files.size(); i++)
    {
        QString filePath = files.at(i).filePath();

        if (files.at(i).isDir())
        {
            SongListItem *item = new SongListItem(SongListItem::DIRECTORY, parentDir, files.at(i).completeBaseName());
            loadSongs(filePath, item);
        }
        else
            addNewSong(filePath, parentDir);
    }

    return parentDir;
}

// ==============================
// ==============================

void Player::addSongs(SongTreeRoot *songs)
{
    QTreeWidget tree;
    tree.addTopLevelItem(songs);

    QTreeWidgetItemIterator it(&tree, QTreeWidgetItemIterator::Selectable);
    while (*it)
    {
        Song *song = reinterpret_cast<Song*>((*it)->data(0, Qt::UserRole).value<quintptr>());
        if (song)
        {
            song->setNum(mp_Songs.size());
            mp_Songs.append(song);
        }

        ++it;
    }

    tree.takeTopLevelItem(0);
}

// ==============================
// ==============================

void Player::removeRemoteSongs()
{
    int i = 0;

    while (i < mp_Songs.size())
    {
        if (mp_Songs.at(i)->isRemote())
            mp_Songs.remove(i);
        else
            i++;
    }

    clientFile.close();
}

// ==============================
// ==============================

void Player::changeSong(int song)
{
    m_CurrentSong = song;

    // Ouverture du fichier
    getCurrentSong().open();

    // Si le player n'est pas stoppé, on le joue
    if (!isStopped())
        getCurrentSong().play();
}

// ==============================
// ==============================

void Player::executeNetworkCommand(CommandRequest *command)
{
    int songNum = command->getSongNum();
    CommandReply *reply = 0;

    switch (command->getCommandType())
    {
        case 'o':
            clientFile.setFileName(mp_Songs.at(songNum)->getFile());
            if (!clientFile.open(QIODevice::ReadOnly))
                reply = new OpenCommandReply(songNum, FMOD_ERR_FILE_NOTFOUND, -1);
            else
            {
                unsigned int fileSize = clientFile.size();
                clientFile.seek(0);

                reply = new OpenCommandReply(songNum, FMOD_OK, fileSize);
            }
            break;

        case 'c':
            clientFile.close();
            reply = new CloseCommandReply(songNum, FMOD_OK);
            break;

        case 'r':
        {
            unsigned int bytesToRead = static_cast<ReadCommandRequest*>(command)->getBytesToRead();
            char *buffer = new char[bytesToRead];
            unsigned int readBytes = clientFile.read(buffer, bytesToRead);

            reply = new ReadCommandReply(songNum, FMOD_OK, buffer, readBytes);
            break;
        }

        case 's':
            if (clientFile.seek(static_cast<SeekCommandRequest*>(command)->getPos()))
                reply = new SeekCommandReply(songNum, FMOD_OK);
            else
                reply = new SeekCommandReply(songNum, FMOD_ERR_FILE_COULDNOTSEEK);
            break;

        default:
            break;
    }

    delete command;

    if (reply)
        emit commandExecuted(reply);
}

