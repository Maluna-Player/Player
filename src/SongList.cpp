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
  // on applique la transformation
  states.transform *= getTransform();

  // on applique la texture du tileset
  states.texture = 0;

  // on dessine les titres
  SongTexts_t::const_iterator songIt;

  for (songIt = m_SongDetails.begin(); songIt != m_SongDetails.end(); ++songIt)
  {
    target.draw(songIt->first, states);
    target.draw(songIt->second, states);
  }
}

// ==============================
// ==============================

bool SongList::collision(int x, int y) const
{
  return (x >= SONG_LIST_X && x <= WINDOW_WIDTH
            && y >= SONG_LIST_Y && y <=(SONG_LIST_Y + (m_SongDetails.size() * SONG_TITLE_H)));
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

void SongList::setCurrentSong(int song)
{
  unsigned int i;

  for (i = 0; i < m_SongDetails.size(); i++)
  {
    if (i == song)
    {
      m_SongDetails.at(i).first.setColor(sf::Color(21, 191, 221));
      m_SongDetails.at(i).second.setColor(sf::Color(21, 191, 221));
    }
    else
    {
      m_SongDetails.at(i).first.setColor(sf::Color::White);
      m_SongDetails.at(i).second.setColor(sf::Color::White);
    }
  }
}

// ==============================
// ==============================

void SongList::add(const std::string& title, const std::string& length)
{
  /* Construction du titre */
  sf::Text titleText;

  titleText.setFont(m_Font);
  titleText.setString(title);
  titleText.setColor(sf::Color::White);
  titleText.setCharacterSize(13);
  titleText.setPosition(sf::Vector2f(SONG_LIST_X, SONG_LIST_Y + (m_SongDetails.size() * SONG_TITLE_H)));

  /* Construction de la durée */
  sf::Text lengthText;

  lengthText.setFont(m_Font);
  lengthText.setString(length);
  lengthText.setColor(sf::Color::White);
  lengthText.setCharacterSize(13);
  lengthText.setPosition(sf::Vector2f(LENGTH_LIST_X, SONG_LIST_Y + (m_SongDetails.size() * SONG_TITLE_H)));

  m_SongDetails.push_back(std::make_pair(titleText, lengthText));
}

// ==============================
// ==============================

void SongList::add(const std::vector<std::pair<std::string, std::string> >& songs)
{
  std::vector<std::pair<std::string, std::string> >::const_iterator songIt;

  for (songIt = songs.begin(); songIt != songs.end(); ++songIt)
    add(songIt->first, songIt->second);
}

// ==============================
// ==============================

void SongList::clear()
{
  m_SongDetails.clear();
}


