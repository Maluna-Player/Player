/*************************************
 * @file    main.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Fonction main du projet.
 * Lance l'application.
 *************************************
*/

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "constants.hpp"

int main(void)
{
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
  window.setVerticalSyncEnabled(true);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color::Black);

    window.display();
  }

  return 0;
}
