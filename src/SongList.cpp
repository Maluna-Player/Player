/*************************************
 * @file    SongList.cpp
 * @date    12/09/14
 * @author  Manuel
 *
 * Définitions de la classe SongList.
 *************************************
*/

#include "SongList.hpp"
#include "constants.hpp"
#include "FileLoadingException.hpp"
#include "Path.hpp"


SongList::SongList()
{
  if (!m_Font.loadFromFile(FONT_FILE))
    throw FileLoadingException("SongList::SongList", FONT_FILE);
}

// ==============================
// ==============================

SongList::~SongList()
{

}

// ==============================
// ==============================

void SongList::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  unsigned int i;

  // on applique la transformation
  states.transform *= getTransform();

  // on applique la texture du tileset
  states.texture = 0;

  // on dessine les titres
  for (i = 0; i < m_Titles.size(); i++)
    target.draw(m_Titles.at(i), states);
}

// ==============================
// ==============================

bool SongList::collision(int x, int y) const
{
  return (x >= SONG_LIST_X && x <= WINDOW_WIDTH
            && y >= SONG_LIST_Y && y <=(SONG_LIST_Y + (m_Titles.size() * SONG_TITLE_H)));
}

// ==============================
// ==============================

int SongList::getClickedSong(int x, int y) const
{
  if (!collision(x, y))
    return -1;
  else
    return ((y - SONG_LIST_Y) / SONG_TITLE_H);
}

// ==============================
// ==============================

void SongList::add(const std::string& title)
{
  sf::Text element;

  element.setFont(m_Font);
  element.setString(title);
  element.setColor(sf::Color::White);
  element.setCharacterSize(13);
  element.setPosition(sf::Vector2f(SONG_LIST_X, SONG_LIST_Y + (m_Titles.size() * SONG_TITLE_H)));

  m_Titles.push_back(element);
}

// ==============================
// ==============================

void SongList::add(const std::vector<std::string>& titles)
{
  unsigned int i;

  for (i = 0; i < titles.size(); i++)
    add(titles.at(i));
}

// ==============================
// ==============================

void SongList::clear()
{
  m_Titles.clear();
}


