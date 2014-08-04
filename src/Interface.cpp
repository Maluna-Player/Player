/*************************************
 * @file    Interface.cpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Définitions de la classe Interface.
 *************************************
*/

#include "Interface.hpp"
#include "constants.hpp"
#include "Fmod.hpp"
#include "Path.hpp"
#include "FileLoadingException.hpp"

/** SFML Time includes **/
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>


Interface::Interface()
{
  m_Window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
  m_Window.setVerticalSyncEnabled(true);
}

// ==============================
// ==============================

Interface::~Interface()
{
  Fmod::deleteInstance();
}

// ==============================
// ==============================

void Interface::changeSong(int song)
{
  if (song != UNDEFINED_SONG)
  {
    m_Player.playSong(song);
    m_SongTitle.setString(Path::baseName(m_Player.getCurrentSong().getFile()));
  }
}

// ==============================
// ==============================

void Interface::run()
{
  int i;
  sf::Clock clock;
  const sf::Time refreshTime = sf::milliseconds(REFRESH_TIME_MS);

  if (!m_Font.loadFromFile(FONT_FILE))
    throw FileLoadingException("Interface::run", FONT_FILE);

  m_SongTitle.setFont(m_Font);
  m_SongTitle.setColor(sf::Color::White);

  if (!m_Player.loadSongs(SONGS_SUBDIR))
    throw FileLoadingException("Interface::run", SONGS_SUBDIR);

  changeSong(FIRST_SONG);

  while (m_Window.isOpen())
  {
    while (m_Window.pollEvent(m_Event))
    {
      switch (m_Event.type)
      {
        case sf::Event::Closed:
          m_Window.close();
          break;

        case sf::Event::KeyPressed:
          if (m_Event.key.code == sf::Keyboard::N)
            changeSong(m_Player.next());
          else if (m_Event.key.code == sf::Keyboard::P)
            changeSong(m_Player.prev());
          else if (m_Event.key.code == sf::Keyboard::L)
            m_Player.setLoop(!m_Player.isLoop());
          break;

        default:
          break;
      }
    }

    if (clock.getElapsedTime() >= refreshTime)
    {
      clock.restart();

      if (!m_Player.isStopped())
      {
        m_Spectrum.update();

        if (m_Player.getCurrentSong().isFinished())
          changeSong(m_Player.next());
      }
    }

    m_Window.clear(sf::Color::Black);

    for (i = 0; i < SPECTRUM_WIDTH; i++)
    {
      m_Window.draw(m_Spectrum.getLine(i));
    }
    m_Window.draw(m_SongTitle);
    m_Window.display();
  }
}
