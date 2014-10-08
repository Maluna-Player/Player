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
#include "ArrayAccessException.hpp"
#include "Path.hpp"


SongList::SongList() : m_Selected(-1), m_Pointed(-1), m_Scroll(0)
{
  if (!m_Font.loadFromFile(FONT_FILE))
    throw FileLoadingException("SongList::SongList", FONT_FILE);

  std::string pointingTexturePath = std::string(IMAGES_SUBDIR) + Path::separator() + "songPointing.png";

  if (!m_PointingTexture.loadFromFile(pointingTexturePath))
    throw FileLoadingException("SongList::SongList", pointingTexturePath);

  m_PointingTexture.setRepeated(true);

  m_PointingBox.setTexture(&m_PointingTexture);
  m_PointingBox.setSize(sf::Vector2f(SONG_LIST_W, SONG_TITLE_H));
  setPosition(sf::Vector2f(SONG_LIST_X, SONG_LIST_Y));
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

  // musique pointée
  if (m_Pointed != -1)
    target.draw(m_PointingBox, states);

  // on dessine les titres
  const int scrolledSongs = m_Scroll / SONG_TITLE_H;
  const int songsToDrawMax = TAB_CONTENT_H / SONG_TITLE_H + 1;

  int drawnSongsNb = 0;
  SongTexts_t::const_iterator songIt = m_SongDetails.begin() + scrolledSongs;

  while (drawnSongsNb < songsToDrawMax && songIt != m_SongDetails.end())
  {
    target.draw(songIt->first, states);
    target.draw(songIt->second, states);

    songIt++;
    drawnSongsNb++;
  }
}

// ==============================
// ==============================

bool SongList::collision(int x, int y) const
{
  return (x >= SONG_LIST_X
          && x <= (SONG_LIST_X + SONG_LIST_W)
          && y >= SONG_LIST_Y
          && y < (SONG_LIST_Y + (m_SongDetails.size() * SONG_TITLE_H))
          && y < (SONG_LIST_Y + SONG_LIST_H));
}

// ==============================
// ==============================

int SongList::getSong(int x, int y) const
{
  if (!collision(x, y))
    return -1;
  else
    return ((y - SONG_LIST_Y + m_Scroll) / SONG_TITLE_H);
}

// ==============================
// ==============================

void SongList::setCurrentSong(int song)
{
  if (m_Selected != -1)
  {
    if (m_Selected >= m_SongDetails.size())
      throw ArrayAccesException("SongList::setCurrentSong", m_SongDetails.size(), m_Selected);

    m_SongDetails.at(m_Selected).first.setColor(sf::Color::White);
    m_SongDetails.at(m_Selected).second.setColor(sf::Color::White);
  }

  if (song >= m_SongDetails.size())
    throw ArrayAccesException("SongList::setCurrentSong", m_SongDetails.size(), song);

  m_Selected = song;
  m_SongDetails.at(m_Selected).first.setColor(sf::Color(21, 191, 221));
  m_SongDetails.at(m_Selected).second.setColor(sf::Color(21, 191, 221));
}

// ==============================
// ==============================

void SongList::setPointedSong(int song)
{
  m_Pointed = song;
  m_PointingBox.setPosition(sf::Vector2f(0, m_Pointed * SONG_TITLE_H - m_Scroll));
}

// ==============================
// ==============================

void SongList::add(const std::string& title, const std::string& length)
{
  int yPos = (m_SongDetails.size() * SONG_TITLE_H) + TEXT_HEIGHT_SPACE;

  /* Construction du titre */
  sf::Text titleText;

  titleText.setFont(m_Font);
  titleText.setString(title);
  titleText.setColor(sf::Color::White);
  titleText.setCharacterSize(13);
  titleText.setPosition(sf::Vector2f(0, yPos));

  /* Construction de la durée */
  sf::Text lengthText;

  lengthText.setFont(m_Font);
  lengthText.setString(length);
  lengthText.setColor(sf::Color::White);
  lengthText.setCharacterSize(13);
  lengthText.setPosition(sf::Vector2f(LENGTH_LIST_X, yPos));

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

  m_Selected = -1;
  m_Pointed = -1;
  m_Scroll = 0;
}

// ==============================
// ==============================

void SongList::scroll(int delta)
{
  const int totalSongListHeight = m_SongDetails.size() * SONG_TITLE_H;

  if (totalSongListHeight > SONG_LIST_H)
  {
    const int scrollMax = totalSongListHeight - SONG_LIST_H;
    int deltaPos = -(delta * SCROLL_INTERVAL);    // Inversion du signe

    if ((m_Scroll + deltaPos) < 0)                // Trop haut
      deltaPos = -m_Scroll;
    else if ((m_Scroll + deltaPos) > scrollMax)   // Trop bas
      deltaPos = scrollMax - m_Scroll;

    if (deltaPos != 0)
    {
      m_Scroll += deltaPos;

      SongTexts_t::iterator songIt;

      for (songIt = m_SongDetails.begin(); songIt != m_SongDetails.end(); ++songIt)
      {
        songIt->first.move(0, -deltaPos);
        songIt->second.move(0, -deltaPos);
      }
    }
  }
}

