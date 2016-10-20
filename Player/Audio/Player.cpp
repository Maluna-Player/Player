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
      m_VolumeState(NB_VOLUME_STATES - 1), mp_PreviewId(nullptr)
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

Player::SongId Player::getNewSongId()
{
    SongId res = m_Cpt;
    m_Cpt++;
    return res;
}

// ==============================
// ==============================

std::shared_ptr<Song> Player::getCurrentSong()
{
    if (m_CurrentSong == UNDEFINED_SONG)
        return nullptr;
    else
        return *m_CurrentSong;
}

// ==============================
// ==============================

Player::SongIt Player::findSong(SongId songId) const
{
    for (auto it = mp_Songs.begin(); it != mp_Songs.end(); ++it)
    {
        if ((*it)->getId() == songId)
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
    if (m_CurrentSong != UNDEFINED_SONG && getCurrentSong()->isAvailable())
    {
        if (m_Stop)
        {
            m_Stop = false;
            getCurrentSong()->play();

            emit stateChanged(PlayerState::PLAY);
        }
        else if (m_Pause)
        {
            m_Pause = false;
            getCurrentSong()->pause(false);

            emit stateChanged(PlayerState::PLAY);
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

        emit stateChanged(PlayerState::STOP);
    }
}

// ==============================
// ==============================

void Player::pause()
{
    if (isPlaying())
    {
        m_Pause = true;

        if (m_CurrentSong != UNDEFINED_SONG)
            getCurrentSong()->pause(true);

        emit stateChanged(PlayerState::PAUSE);
    }
}

// ==============================
// ==============================

void Player::mute(bool mute)
{
    m_Mute = mute;
    FmodManager::getInstance().setMute(mute);
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
    FmodManager::getInstance().setVolume(volume);
}

// ==============================
// ==============================

void Player::clearSongs(SongList_t list)
{
    mp_Songs.clear(list);
}

// ==============================
// ==============================

std::shared_ptr<Song> Player::getLocalSong(const QString& filePath) const
{
    auto localSongs = mp_Songs.getSubSets(SongList_t::LOCAL_SONGS);

    for (auto song : localSongs)
    {
        if (song->getFile() == filePath)
            return song;
    }

    return nullptr;
}

// ==============================
// ==============================

std::shared_ptr<network::RemoteSong> Player::getRemoteSong(const SongId id) const
{
    auto remoteSongs = mp_Songs.getSubSets(SongList_t::REMOTE_SONGS);

    for (auto song : remoteSongs)
    {
        auto remoteSong = std::static_pointer_cast<network::RemoteSong>(song);
        if (remoteSong->getRemoteId() == id)
            return remoteSong;
    }

    return nullptr;
}

// ==============================
// ==============================

std::shared_ptr<Song> Player::createLocalSong(const QString& filePath, bool inFolder)
{
    QFileInfo fileInfo(filePath);
    QString absoluteFilePath = fileInfo.canonicalFilePath();

    std::shared_ptr<Song> song = getLocalSong(absoluteFilePath);
    if (!song)
    {
        try
        {
            SongId id = getNewSongId();
            song.reset(new Song(id, absoluteFilePath, inFolder));
            SongList_t list = inFolder ? SongList_t::DIRECTORY_SONGS : SongList_t::IMPORTED_SONGS;

            mp_Songs[list][id] = song;
        }
        catch (FmodManager::StreamError error)
        {
            if (error == FmodManager::StreamError::FILE_ERROR)
                qWarning() << "Error loading" << absoluteFilePath;
            else if (error == FmodManager::StreamError::FORMAT_ERROR)
                qWarning() << "Unsupported format for" << absoluteFilePath;
        }
    }

    song->setAvailable(true);

    return song;
}

// ==============================
// ==============================

std::shared_ptr<network::RemoteSong> Player::createRemoteSong(const QString& file, SongId remoteId, SoundPos_t length, const QString& artist, SoundSettings *settings)
{
    std::shared_ptr<network::RemoteSong> remoteSong = getRemoteSong(remoteId);

    if (!remoteSong)
    {
        SongId id = getNewSongId();
        remoteSong.reset(new network::RemoteSong(id, file, remoteId, length, artist, settings));

        mp_Songs[SongList_t::REMOTE_SONGS][id] = remoteSong;
    }

    return remoteSong;
}

// ==============================
// ==============================

gui::SongListItem* Player::addNewSong(SongList_t list, const QString& filePath, gui::SongListItem *parentDir)
{
    gui::SongListItem *item = nullptr;

    bool inFolder = (list == SongList_t::DIRECTORY_SONGS);
    std::shared_ptr<Song> song = createLocalSong(filePath, inFolder);

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

gui::SongTreeRoot* Player::reloadSongs(const QString& dirPath)
{
    for (auto song : mp_Songs[SongList_t::DIRECTORY_SONGS])
        song.second->setAvailable(false);

    auto songTree = loadSongs(dirPath);

    auto it = mp_Songs.begin();
    while (it != mp_Songs.end())
    {
        if ((*it)->isInFolder() && !((*it)->isAvailable()))
            it = mp_Songs.erase(it);
        else
            ++it;
    }

    return songTree;
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
    do
    {
        changeSong(prev());
    }
    while (isPlaying() && !getCurrentSong()->isAvailable());
}

// ==============================
// ==============================

void Player::nextSong()
{
    do
    {
        changeSong(next());
    }
    while (isPlaying() && !getCurrentSong()->isAvailable());
}

// ==============================
// ==============================

bool Player::changeSong(SongIt song)
{
    if (song != UNDEFINED_SONG)
    {
        m_CurrentSong = song;

        if (!getCurrentSong()->isAvailable())
        {
            emit songChanged();
            return false;
        }

        try
        {
            // Ouverture du fichier
            getCurrentSong()->open();

            // Si le player n'est pas stoppé, on le joue
            if (!isStopped())
                getCurrentSong()->play();

            emit songChanged();
        }
        catch (FmodManager::StreamError error)
        {
            getCurrentSong()->setAvailable(false);
            emit streamError(getCurrentSong()->getId());
            emit songChanged();

            return false;
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

bool Player::changeSong(SongId songId)
{
    if (!changeSong(findSong(songId)))
    {
        if (m_CurrentSong != UNDEFINED_SONG && !getCurrentSong()->isAvailable())
            stop();

        return false;
    }

    return true;
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

    if (mp_PreviewId != nullptr && getPreviewPosition() >= getPreviewLength())
    {
        stopPreview();
        emit previewFinished();
    }

    FmodManager::getInstance().update();
}

// ==============================
// ==============================

bool Player::isPreviewing() const
{
    return (mp_PreviewId != nullptr && FmodManager::getInstance().isPlaying(*mp_PreviewId));
}

// ==============================
// ==============================

SoundPos_t Player::getPreviewPosition() const
{
    if (!mp_PreviewId)
        return 0;

    return FmodManager::getInstance().getSoundPosition(*mp_PreviewId);
}

// ==============================
// ==============================

SoundPos_t Player::getPreviewLength() const
{
    if (!mp_PreviewId)
        return 0;

    return FmodManager::getInstance().getSoundLength(*mp_PreviewId);
}

// ==============================
// ==============================

void Player::startPreview(const QString& filePath)
{
    pause();

    mp_PreviewId = std::make_unique<SoundID_t>(FmodManager::getInstance().openFromFile(filePath.toStdString(), false));
    FmodManager::getInstance().playSound(*mp_PreviewId);
}

// ==============================
// ==============================

void Player::stopPreview()
{
    if (mp_PreviewId)
    {
        FmodManager::getInstance().stopSound(*mp_PreviewId);
        mp_PreviewId.reset(nullptr);

        play();
    }
}

// ==============================
// ==============================

void Player::executeNetworkCommand(std::shared_ptr<network::commands::CommandRequest> command)
{
    SongId songId = command->getSongId();
    std::shared_ptr<network::commands::CommandReply> reply { nullptr };

    switch (command->getCommandType())
    {
        case 'o':
        {
            if (clientFile.isOpen())
                clientFile.close();

            SongIt song = findSong(songId);
            if (song != UNDEFINED_SONG)
            {
                clientFile.setFileName((*song)->getFile());
                clientFile.open(QIODevice::ReadOnly);
            }

            if (!clientFile.isOpen())
                reply = std::make_shared<network::commands::OpenCommandReply>(songId, FMOD_ERR_FILE_NOTFOUND, 0);
            else
            {
                unsigned int fileSize = clientFile.size();
                clientFile.seek(0);

                reply = std::make_shared<network::commands::OpenCommandReply>(songId, FMOD_OK, fileSize);
            }
            break;
        }

        case 'c':
            clientFile.close();
            reply = std::make_shared<network::commands::CloseCommandReply>(songId, FMOD_OK);
            break;

        case 'r':
        {
            unsigned int bytesToRead = std::static_pointer_cast<network::commands::ReadCommandRequest>(command)->getBytesToRead();
            char *buffer = new char[bytesToRead];
            unsigned int readBytes = clientFile.read(buffer, bytesToRead);

            reply = std::make_shared<network::commands::ReadCommandReply>(songId, FMOD_OK, buffer, readBytes);
            break;
        }

        case 's':
            if (clientFile.seek(std::static_pointer_cast<network::commands::SeekCommandRequest>(command)->getPos()))
                reply = std::make_shared<network::commands::SeekCommandReply>(songId, FMOD_OK);
            else
                reply = std::make_shared<network::commands::SeekCommandReply>(songId, FMOD_ERR_FILE_COULDNOTSEEK);
            break;

        default:
            break;
    }

    if (reply)
        emit commandExecuted(reply);
}


} // audio
