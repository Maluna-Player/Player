/*************************************
 * @file    main.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Fonction main du projet.
 * Lance l'application.
 *************************************
*/

/** SFML Window includes **/
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

/** SFML Graphics includes **/
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

/** SFML Time includes **/
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include "constants.hpp"
#include "Player.hpp"
#include "Fmod.hpp"
#include "Path.hpp"

int main(void)
{
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
  window.setVerticalSyncEnabled(true);

  sf::Clock clock;
  sf::Time refreshTime = sf::milliseconds(REFRESH_TIME_MS);

  sf::Font font;
  if (!font.loadFromFile(FONT_FILE))
    return -1;

  sf::Text songTitle;
  songTitle.setFont(font);
  songTitle.setColor(sf::Color::White);

  Player p;
  if (!p.loadSongs(SONGS_SUBDIR))
    return -1;

  p.playAllSongs();
  songTitle.setString(Path::baseName(p.getCurrentSong().getFile()));

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if (clock.getElapsedTime() >= refreshTime)
    {
      clock.restart();

      if (!p.isStopped() && p.getCurrentSong().isFinished())
      {
        if (p.nextSong())
          songTitle.setString(Path::baseName(p.getCurrentSong().getFile()));
      }
    }

    window.clear(sf::Color::Black);
    window.draw(songTitle);
    window.display();
  }

  Fmod::deleteInstance();

  return 0;
}
