/*************************************
 * @file    Player.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Player.
 *************************************
*/

#include "Player.hpp"
#include "ArrayAccessException.hpp"
#include "Fmod.hpp"

#include <dirent.h>
#include <cstdio>
#include <cerrno>


Player::Player()
  : m_CurrentSong(0), m_Playlist(true), m_Loop(false),
    m_Stop(false)
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

const Song& Player::getCurrentSong() const
{
  if (m_CurrentSong >= m_Songs.size())
    throw ArrayAccesException("Player::getCurrentSong", m_Songs.size(), m_CurrentSong);

  return m_Songs.at(m_CurrentSong);
}

// ==============================
// ==============================

void Player::play()
{
  m_Stop = false;
  getCurrentSong().play();
}

// ==============================
// ==============================

void Player::stop()
{
  m_Stop = true;
  Fmod::getInstance()->stopSound(0);
}

// ==============================
// ==============================

bool Player::isStopped() const
{
  return m_Stop;
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
  else if (m_Loop)
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
  else if (m_Loop)
    return FIRST_SONG;
  else
    return UNDEFINED_SONG;
}

// ==============================
// ==============================

bool Player::loadSongs(const std::string& dir)
{
  DIR *rep = 0;

  /* Ouverture du répertoire */
  if ((rep = opendir(dir.c_str())) == 0)
  {
    std::perror("opendir");
    return false;
  }

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
      std::string filePath = dir + "\\" + file->d_name;

      m_Songs.push_back(Song(filePath, songsNb));
    }
  }

  /* Erreur lors du parcours */
  if (errno)
  {
    std::perror("readdir");
    return false;
  }

  /* Fermeture du répertoire */
  if (closedir(rep) == -1)
  {
    std::perror("closedir");
    return false;
  }

  return true;
}

// ==============================
// ==============================

void Player::playSong(int song)
{
  m_CurrentSong = song;

  if (!m_Stop)
    getCurrentSong().play();
}

