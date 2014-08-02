/*************************************
 * @file    Interface.hpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Déclarations de la classe Interface
 * contenant le lancement et la gestion
 * de la fenêtre de l'application.
 *************************************
*/

#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

/** SFML Window includes **/
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

/** SFML Graphics includes **/
#include <SFML/Graphics/Font.hpp>

#include "Player.hpp"

class Interface
{
  private:

    sf::RenderWindow m_Window;
    sf::Event m_Event;

    sf::Font m_Font;

    Player m_Player;

  public:

    Interface();
    virtual ~Interface();

    /**
     * Lance la boucle principale du programme.
    */
    virtual void run();
};

#endif  // __INTERFACE_HPP__
