/*************************************
 * @file    main.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Fonction main du projet.
 * Lance l'application.
 *************************************
*/

/** SFML Window/Graphics includes **/
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

/** SFML Time includes **/
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include "constants.hpp"
#include "Player.hpp"
#include "Fmod.hpp"

int main(void)
{
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
  window.setVerticalSyncEnabled(true);

  sf::Clock clock;
  sf::Time refreshTime = sf::milliseconds(REFRESH_TIME_MS);

  Player p;
  if (!p.loadSongs(SONGS_SUBDIR))
    return -1;

  p.playAllSongs();

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

      if (!p.isStopped())
      {
        if (p.getCurrentSong().isFinished())
          p.nextSong();
      }
    }

    window.clear(sf::Color::Black);

    window.display();
  }

  Fmod::deleteInstance();

  return 0;
}
