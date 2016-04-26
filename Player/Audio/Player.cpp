/*************************************
 * @file    Player.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Player.
 *************************************
*/

#include "Player.h"
#include "Song.h"
#include "../Network/RemoteSong.h"
#include "../Exceptions/LibException.h"
#include "../Exceptions/FileLoadingException.h"
#include "../Network/Commands/CommandRequest.h"
#include "../Network/Commands/CommandReply.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include "../Gui/SongListIterator.h"


namespace audio {


Player::Player()
    : m_Cpt(0), mp_Songs({SongList_t::DIRECTORY_SONGS, SongList_t::IMPORTED_SONGS, SongList_t::REMOTE_SONGS}),
      m_CurrentSong(UNDEFINED_SONG), m_Playlist(true), m_Loop(false),
      m_Pause(false), m_Stop(true), m_Mute(false),
      m_VolumeState(NB_VOLUME_STATES - 1)
{

}

// ==============================
// ==============================

Player::~Player()
{
    clearSongs();
    mp_Songs.clear();
}

// ==============================
// ==============================

Player::SongId Player::getNewSongNum()
{
    SongId res = m_Cpt;
    m_Cpt++;
    return res;
}

// ==============================
// ==============================

Song* Player::getCurrentSong()
{
    if (m_CurrentSong == UNDEFINED_SONG)
        return nullptr;
    else
        return *m_CurrentSong;
}

// ==============================
// ==============================

Player::SongIt Player::findSong(SongId song) const
{
    for (auto it = mp_Songs.begin(); it != mp_Songs.end(); ++it)
    {
        if ((*it)->getNum() == song)
            return it;
    }

    return UNDEFINED_SONG;
}

// ==============================
// ==============================

int Player::songsCount(SongList_t list) const
{
    return mp_Songs.getSubSets(list).elementsCount();
}

// ==============================
// ==============================

void Player::play()
{
    if (m_CurrentSong != UNDEFINED_SONG)
    {
        if (m_Stop)
        {
            m_Stop = false;
            getCurrentSong()->play();
        }
        else if (m_Pause)
        {
            m_Pause = false;
            getCurrentSong()->pause(false);
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

        if (m_CurrentSong != UNDEFINED_SONG)
            getCurrentSong()->stop();

        emit stopped();
    }
}

// ==============================
// ==============================

void Player::pause()
{
    m_Pause = true;

    if (m_CurrentSong != UNDEFINED_SONG)
        getCurrentSong()->pause(true);
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
    return (!m_Pause && !m_Stop && m_CurrentSong != UNDEFINED_SONG);
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

Player::SongIt Player::first() const
{
    return FIRST_SONG;
}

// ==============================
// ==============================

Player::SongIt Player::prev() const
{
    SongIt prev = (m_Loop && m_CurrentSong == FIRST_SONG) ? mp_Songs.end() : m_CurrentSong;

    --prev;
    return prev;
}

// ==============================
// ==============================

Player::SongIt Player::next() const
{
    SongIt next = m_CurrentSong;
    ++next;

    if (m_Loop && next == mp_Songs.end())
        next = FIRST_SONG;

    return next;
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

void Player::clearSongs(SongList_t list)
{
    auto songs = mp_Songs.getSubSets(list);

    for (Song *song : songs)
        delete song;

    mp_Songs.clear(list);
}

// ==============================
// ==============================

bool Player::containsLocalSong(const QString& filePath) const
{
    auto localSongs = mp_Songs.getSubSets(SongList_t::LOCAL_SONGS);

    for (auto song : localSongs)
    {
        if (song->getFile() == filePath)
            return true;
    }

    return false;
}

// ==============================
// ==============================

bool Player::containsRemoteSong(const SongId num) const
{
    auto remoteSongs = mp_Songs.getSubSets(SongList_t::REMOTE_SONGS);

    for (auto song : remoteSongs)
    {
        if (static_cast<network::RemoteSong*>(song)->getRemoteNum() == num)
            return true;
    }

    return false;
}

// ==============================
// ==============================

Song* Player::createLocalSong(const QString& filePath, bool inFolder)
{
    Song *song = nullptr;

    QFileInfo fileInfo(filePath);
    QString absoluteFilePath = fileInfo.canonicalFilePath();

    if (!containsLocalSong(absoluteFilePath))
    {
        try
        {
            SongId num = getNewSongNum();
            song = new Song(num, absoluteFilePath, inFolder);
            SongList_t list = inFolder ? SongList_t::DIRECTORY_SONGS : SongList_t::IMPORTED_SONGS;

            mp_Songs[list][num] = song;
        }
        catch (FmodManager::StreamError error)
        {
            if (error == FmodManager::StreamError::FILE_ERROR)
                qWarning() << "Error loading" << absoluteFilePath;
            else if (error == FmodManager::StreamError::FORMAT_ERROR)
                qWarning() << "Unsupported format for" << absoluteFilePath;

            if (song)
            {
                delete song;
                song = nullptr;
            }
        }
    }

    return song;
}

// ==============================
// ==============================

network::RemoteSong* Player::createRemoteSong(const QString& file, SongId remoteNum, SoundPos_t length, const QString& artist, SoundSettings *settings)
{
    network::RemoteSong *song = nullptr;

    if (!containsRemoteSong(remoteNum))
    {
        SongId num = getNewSongNum();
        song = new network::RemoteSong(num, file, remoteNum, length, artist, settings);

        mp_Songs[SongList_t::REMOTE_SONGS][num] = song;
    }

    return song;
}

// ==============================
// ==============================

gui::SongListItem* Player::addNewSong(SongList_t list, const QString& filePath, gui::SongListItem *parentDir)
{
    gui::SongListItem *item = nullptr;

    bool inFolder = (list == SongList_t::DIRECTORY_SONGS);
    Song *song = createLocalSong(filePath, inFolder);

    if (song)
    {
        QFileInfo fileInfo(filePath);
        item = new gui::SongListItem(gui::SongListItem::ElementType::SONG, parentDir, fileInfo.completeBaseName());
        item->setAttachedSong(song);
    }

    return item;
}

// ==============================
// ==============================

gui::SongTreeRoot* Player::loadSongs(const QString& dirPath, gui::SongTreeRoot *parentDir)
{
    if (!parentDir)
        parentDir = new gui::SongTreeRoot(gui::SongListItem::ElementType::ROOT);

    QDir dir(dirPath);
    if (!dir.exists())
        throw exceptions::FileLoadingException("Player::loadSongs", dirPath.toStdString());

    QFileInfoList files = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (const QFileInfo& fileInfo : files)
    {
        QString filePath = fileInfo.filePath();

        if (fileInfo.isDir())
        {
            gui::SongListItem *item = new gui::SongListItem(gui::SongListItem::ElementType::DIRECTORY, nullptr, fileInfo.completeBaseName());
            loadSongs(filePath, item);

            if (item->childCount() > 0)
                item->setParent(parentDir);
        }
        else
            addNewSong(SongList_t::DIRECTORY_SONGS, filePath, parentDir);
    }

    return parentDir;
}

// ==============================
// ==============================

void Player::firstSong()
{
    changeSong(first());
}

// ==============================
// ==============================

void Player::previousSong()
{
    changeSong(prev());
}

// ==============================
// ==============================

void Player::nextSong()
{
    changeSong(next());
}

// ==============================
// ==============================

bool Player::changeSong(SongIt song)
{
    if (song != UNDEFINED_SONG)
    {
        m_CurrentSong = song;

        try
        {
            // Ouverture du fichier
            getCurrentSong()->open();

            // Si le player n'est pas stoppé, on le joue
            if (!isStopped())
                getCurrentSong()->play();

            emit songChanged();
        }
        catch(FmodManager::StreamError error)
        {
            emit streamError(getCurrentSong()->getFile());
        }
    }
    else
    {
        stop();
    }

    return (m_CurrentSong != UNDEFINED_SONG);
}

// ==============================
// ==============================

bool Player::changeSong(SongId song)
{
    return changeSong(findSong(song));
}

// ==============================
// ==============================

void Player::update()
{
    if (isPlaying())
    {
        if (getCurrentSong()->isFinished())
            nextSong();
    }

    FmodManager::getInstance()->update();
}

// ==============================
// ==============================

void Player::executeNetworkCommand(network::commands::CommandRequest *command)
{
    SongId songNum = command->getSongNum();
    network::commands::CommandReply *reply = nullptr;

    switch (command->getCommandType())
    {
        case 'o':
        {
            SongIt song = findSong(songNum);
            clientFile.setFileName((*song)->getFile());
            if (!clientFile.open(QIODevice::ReadOnly))
                reply = new network::commands::OpenCommandReply(songNum, FMOD_ERR_FILE_NOTFOUND, -1);
            else
            {
                unsigned int fileSize = clientFile.size();
                clientFile.seek(0);

                reply = new network::commands::OpenCommandReply(songNum, FMOD_OK, fileSize);
            }
            break;
        }

        case 'c':
            clientFile.close();
            reply = new network::commands::CloseCommandReply(songNum, FMOD_OK);
            break;

        case 'r':
        {
            unsigned int bytesToRead = static_cast<network::commands::ReadCommandRequest*>(command)->getBytesToRead();
            char *buffer = new char[bytesToRead];
            unsigned int readBytes = clientFile.read(buffer, bytesToRead);

            reply = new network::commands::ReadCommandReply(songNum, FMOD_OK, buffer, readBytes);
            break;
        }

        case 's':
            if (clientFile.seek(static_cast<network::commands::SeekCommandRequest*>(command)->getPos()))
                reply = new network::commands::SeekCommandReply(songNum, FMOD_OK);
            else
                reply = new network::commands::SeekCommandReply(songNum, FMOD_ERR_FILE_COULDNOTSEEK);
            break;

        default:
            break;
    }

    delete command;

    if (reply)
        emit commandExecuted(reply);
}


} // audio
