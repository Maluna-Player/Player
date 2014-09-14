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
  SoundID_t id = Fmod::getInstance()->openFromFile(m_File);
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

int Song::getNum() const
{
  return m_Num;
}

// ==============================
// ==============================

std::string Song::getFile() const
{
  return m_File;
}

// ==============================
// ==============================

SoundPos_t Song::getLength() const
{
  return m_Length;
}

// ==============================
// ==============================

void Song::open()
{
  m_SoundID = Fmod::getInstance()->openFromFile(m_File);
}

// ==============================
// ==============================

void Song::play() const
{
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

SoundPos_t Song::getPosition() const
{
  return Fmod::getInstance()->getSoundPosition(m_SoundID);
}

// ==============================
// ==============================

void Song::setPosition(SoundPos_t pos) const
{
  if (pos >= m_Length)
    pos = m_Length - 1;

  Fmod::getInstance()->setSoundPosition(m_SoundID, pos);
}

// ==============================
// ==============================

bool Song::isFinished() const
{
  return !(Fmod::getInstance()->isPlaying(m_SoundID));
}

