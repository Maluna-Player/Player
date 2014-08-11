/*************************************
 * @file    Song.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Song.
 *************************************
*/

#include "Song.hpp"


Song::Song(const std::string& file, int num)
  : m_File(file), m_Num(num), m_SoundID(0)
{
  SoundID_t id = Fmod::getInstance()->openSound(m_File);
  m_Length = Fmod::getInstance()->getSoundLength(id);
}

// ==============================
// ==============================

Song::Song(const Song& song)
  : m_File(song.m_File), m_Num(song.m_Num), m_Length(song.m_Length), m_SoundID(0)
{

}

// ==============================
// ==============================

Song::~Song()
{

}

// ==============================
// ==============================

SoundID_t Song::getSoundID() const
{
  return m_SoundID;
}

// ==============================
// ==============================

std::string Song::getFile() const
{
  return m_File;
}

// ==============================
// ==============================

unsigned int Song::getLength() const
{
  return m_Length;
}

// ==============================
// ==============================

void Song::play()
{
  m_SoundID = Fmod::getInstance()->openSound(m_File);
  Fmod::getInstance()->playSound(m_SoundID);
}

// ==============================
// ==============================

void Song::pause(bool paused) const
{
  Fmod::getInstance()->pauseSound(m_SoundID, paused);
}

// ==============================
// ==============================

void Song::stop() const
{
  Fmod::getInstance()->stopSound(m_SoundID);
}

// ==============================
// ==============================

bool Song::isFinished() const
{
  return !(Fmod::getInstance()->isPlaying(m_SoundID));
}
