/*************************************
 * @file    Song.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Song.
 *************************************
*/

#include "Song.hpp"
#include "Fmod.hpp"


Song::Song(const std::string& file, int num)
  : m_File(file), m_Num(num)
{
  Fmod::getInstance()->openSound(m_File);
  m_Length = Fmod::getInstance()->getSoundLength();
}

// ==============================
// ==============================

Song::Song(const Song& song)
  : m_File(song.m_File), m_Num(song.m_Num), m_Length(song.m_Length)
{

}

// ==============================
// ==============================

Song::~Song()
{

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

void Song::play() const
{
  Fmod::getInstance()->openSound(m_File);
  Fmod::getInstance()->playSound();
}

// ==============================
// ==============================

bool Song::isFinished() const
{
  return (Fmod::getInstance()->getSoundPosition() >= m_Length);
}
