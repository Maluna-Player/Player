/*************************************
 * @file    Player.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Player.
 *************************************
*/

#include "Player.h"
#include "ArrayAccessException.h"
#include "LibException.h"
#include "FileLoadingException.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>


Player::Player()
    : mp_SongTree(0), m_CurrentSong(0), m_Playlist(true), m_Loop(false),
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

    if (mp_SongTree)
        delete mp_SongTree;
}

// ==============================
// ==============================

Song& Player::getCurrentSong()
{
    if (m_CurrentSong >= mp_Songs.size())
        throw ArrayAccesException("Player::getCurrentSong", mp_Songs.size(), m_CurrentSong);

    return *(mp_Songs[m_CurrentSong]);
}

// ==============================
// ==============================

QList<QTreeWidgetItem*> Player::getSongDetails() const
{
    return mp_SongTree->takeChildren();
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

    if (mp_SongTree)
        delete mp_SongTree;
}

// ==============================
// ==============================

void Player::loadSongs(const QString& dirPath, QTreeWidgetItem *parentDir)
{
    if (!parentDir)
        parentDir = new QTreeWidgetItem;

    QDir dir(dirPath);
    if (!dir.exists())
        throw FileLoadingException("Player::loadSongs", dirPath.toStdString());

    QFileInfoList files = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (int i = 0; i < files.size(); i++)
    {
        QString filePath = files.at(i).filePath();

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, files.at(i).completeBaseName());

        if (files.at(i).isDir())
        {
            parentDir->addChild(item);
            loadSongs(filePath, item);
        }
        else
        {
            try
            {
                FmodManager::getInstance()->openFromFile(filePath.toStdString());
                Song *song = new Song(filePath, i+1);

                mp_Songs.append(song);
                item->setData(0, Qt::UserRole, reinterpret_cast<quintptr>(song));
                parentDir->addChild(item);
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
    }

    mp_SongTree = parentDir;
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

