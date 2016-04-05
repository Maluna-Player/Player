/*************************************
 * @file    Player.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Player.
 *************************************
*/

#include "Player.h"
#include "../Exceptions/LibException.h"
#include "../Exceptions/FileLoadingException.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include "../Gui/SongListIterator.h"


Player::Player()
    : mp_Songs({Constants::DIRECTORY_SONGS, Constants::IMPORTED_SONGS, Constants::REMOTE_SONGS}),
      m_CurrentSong(UNDEFINED_SONG), m_Playlist(true), m_Loop(false),
      m_Pause(false), m_Stop(true), m_Mute(false),
      m_VolumeState(Constants::NB_VOLUME_STATES - 1)
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

Song* Player::getCurrentSong()
{
    if (m_CurrentSong == UNDEFINED_SONG)
        return nullptr;
    else
        return *m_CurrentSong;
}

// ==============================
// ==============================

Player::SongIt Player::findSong(int song) const
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

int Player::songsCount(Constants::SongList_t list) const
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

    float volume = static_cast<float>(volumeState) / (Constants::NB_VOLUME_STATES - 1);
    FmodManager::getInstance()->setVolume(volume);
}

// ==============================
// ==============================

void Player::clearSongs(Constants::SongList_t list)
{
    auto songs = mp_Songs.getSubSets(list);

    for (auto it = songs.begin(); it != songs.end(); ++it)
        delete *it;

    mp_Songs.clear(list);
}

// ==============================
// ==============================

bool Player::containsSong(const QString& filePath) const
{
    auto songs = mp_Songs.getSubSets(Constants::LOCAL_SONGS);

    for (auto it = songs.begin(); it != songs.end(); ++it)
    {
        if ((*it)->getFile() == filePath)
            return true;
    }

    return false;
}

// ==============================
// ==============================

SongListItem* Player::addNewSong(Constants::SongList_t list, const QString& filePath, SongListItem *parentDir)
{
    QFileInfo fileInfo(filePath);
    QString absoluteFilePath = fileInfo.canonicalFilePath();
    SongListItem *item = 0;

    if (!containsSong(absoluteFilePath))
    {
        item = new SongListItem(SongListItem::SONG, parentDir, fileInfo.completeBaseName());

        try
        {
            bool inFolder = (list == Constants::DIRECTORY_SONGS);
            Song *song = new Song(absoluteFilePath, songsCount(), inFolder);

            mp_Songs[list][song->getNum()] = song;
            item->setAttachedSong(song);
        }
        catch (FmodManager::StreamError_t error)
        {
            if (error == FmodManager::FILE_ERROR)
                qWarning() << "Error loading" << absoluteFilePath;
            else if (error == FmodManager::FORMAT_ERROR)
                qWarning() << "Unsupported format for" << absoluteFilePath;

            delete item;
            item = 0;
        }
    }

    return item;
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
            SongListItem *item = new SongListItem(SongListItem::DIRECTORY, 0, files.at(i).completeBaseName());
            loadSongs(filePath, item);

            if (item->childCount() > 0)
                item->setParent(parentDir);
        }
        else
            addNewSong(Constants::DIRECTORY_SONGS, filePath, parentDir);
    }

    return parentDir;
}

// ==============================
// ==============================

void Player::addSongs(Constants::SongList_t list, SongTreeRoot *songs)
{
    QTreeWidget tree;
    tree.addTopLevelItem(songs);

    SongListIterator it(&tree, QTreeWidgetItemIterator::Selectable);
    while (!it.isNull())
    {
        Song *song = (*it)->getAttachedSong();
        if (song)
        {
            song->setNum(songsCount());
            mp_Songs[list][song->getNum()] = song;
        }

        ++it;
    }

    tree.takeTopLevelItem(0);
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
        catch(FmodManager::StreamError_t error)
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

bool Player::changeSong(int song)
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

        FmodManager::getInstance()->update();
    }
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
        {
            SongIt song = findSong(songNum);
            clientFile.setFileName((*song)->getFile());
            if (!clientFile.open(QIODevice::ReadOnly))
                reply = new OpenCommandReply(songNum, FMOD_ERR_FILE_NOTFOUND, -1);
            else
            {
                unsigned int fileSize = clientFile.size();
                clientFile.seek(0);

                reply = new OpenCommandReply(songNum, FMOD_OK, fileSize);
            }
            break;
        }

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

