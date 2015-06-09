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
#include "Path.h"

#include <dirent.h>
#include <cstring>
#include <cerrno>


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
    m_Songs.clear();
}

// ==============================
// ==============================

Song& Player::getCurrentSong()
{
    if (m_CurrentSong >= m_Songs.size())
        throw ArrayAccesException("Player::getCurrentSong", m_Songs.size(), m_CurrentSong);

    return m_Songs[m_CurrentSong];
}

// ==============================
// ==============================

QVector<QPair<QString, int> > Player::getSongDetails() const
{
    int i;
    QVector<QPair<QString, int> > songs;

    for (i = 0; i < m_Songs.size(); i++)
        songs.append(qMakePair(m_Songs.at(i).getTitle(), m_Songs.at(i).getLength()));

    return songs;
}

// ==============================
// ==============================

void Player::play()
{
    if (m_Songs.size() > 0)
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

        if (m_Songs.size() > 0)
            getCurrentSong().stop();
    }
}

// ==============================
// ==============================

void Player::pause()
{
    m_Pause = true;

    if (m_Songs.size() > 0)
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
    if (m_Songs.size() > 0)
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
    else if (m_Loop && m_Songs.size() > 0)
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
    else if (m_Loop && m_Songs.size() > 0)
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

void Player::loadSongs(const QString& dir)
{
    DIR *rep = 0;

    /* Ouverture du répertoire */
    if ((rep = opendir(dir.toStdString().c_str())) == 0)
        throw LibException("Player::loadSongs", "opendir", std::strerror(errno));

    m_Songs.clear();

    struct dirent* file = 0;
    errno = 0;
    int songsNb = 0;

    /* Parcours des fichiers du répertoire */
    while ((file = readdir(rep)) != 0)
    {
        // fichier trouvé
        if (std::string(file->d_name) != "." && std::string(file->d_name) != "..")
        {
            songsNb++;
            QString filePath = dir + Path::separator() + QString(file->d_name);

            m_Songs.append(Song(filePath, songsNb));
        }
    }

    /* Erreur lors du parcours */
    if (errno)
        throw LibException("Player::loadSongs", "readdir", std::strerror(errno));

    /* Fermeture du répertoire */
    if (closedir(rep) == -1)
        throw LibException("Player::loadSongs", "closedir", std::strerror(errno));
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

