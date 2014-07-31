/*************************************
 * @file    Player.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Player.
 *************************************
*/

#include "Player.hpp"

#include <dirent.h>
#include <cstdio>
#include <cerrno>


Player::Player()
  : m_CurrentSong(0), m_Playlist(true), m_Loop(false),
    m_Stop(true)
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
  return m_Songs.at(m_CurrentSong);
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

void Player::playAllSongs(bool loop)
{
  m_Playlist = true;
  m_Loop = loop;
  m_Stop = false;
  m_CurrentSong = 0;

  getCurrentSong().play();
}

// ==============================
// ==============================

void Player::nextSong()
{
  if (m_Playlist)
  {
    if (m_CurrentSong < m_Songs.size() - 1)
    {
      m_CurrentSong++;
      getCurrentSong().play();
    }
    else if (m_Loop)
    {
      m_CurrentSong = 0;
      getCurrentSong().play();
    }
    else
    {
      m_Stop = true;
    }
  }
}

// ==============================
// ==============================

bool Player::isStopped() const
{
  return m_Stop;
}
