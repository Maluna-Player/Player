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

#include <iostream>
#include <cstdlib>

#include <SFML/Graphics/Text.hpp>

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

void Interface::printInterfaceError(const std::string& message) const
{
  std::cerr << message << std::endl;
  std::exit(EXIT_FAILURE);
}

// ==============================
// ==============================

void Interface::run()
{
  sf::Clock clock;
  const sf::Time refreshTime = sf::milliseconds(REFRESH_TIME_MS);

  if (!m_Font.loadFromFile(FONT_FILE))
    printInterfaceError("Cannot open " + std::string(FONT_FILE));

  sf::Text songTitle;
  songTitle.setFont(m_Font);
  songTitle.setColor(sf::Color::White);

  if (!m_Player.loadSongs(SONGS_SUBDIR))
    printInterfaceError("Error loading files in " + std::string(SONGS_SUBDIR) + " directory");

  m_Player.playAllSongs();
  songTitle.setString(Path::baseName(m_Player.getCurrentSong().getFile()));

  while (m_Window.isOpen())
  {
    while (m_Window.pollEvent(m_Event))
    {
      switch (m_Event.type)
      {
        case sf::Event::Closed:
          m_Window.close();
          break;

        default:
          break;
      }
    }

    if (clock.getElapsedTime() >= refreshTime)
    {
      clock.restart();

      if (!m_Player.isStopped() && m_Player.getCurrentSong().isFinished())
      {
        if (m_Player.nextSong())
          songTitle.setString(Path::baseName(m_Player.getCurrentSong().getFile()));
      }
    }

    m_Window.clear(sf::Color::Black);
    m_Window.draw(songTitle);
    m_Window.display();
  }
}
