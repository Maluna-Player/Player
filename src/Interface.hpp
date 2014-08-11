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
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Player.hpp"
#include "Spectrum.hpp"
#include "CircleButton.hpp"

class Interface
{
  private:

    sf::RenderWindow m_Window;
    sf::Event m_Event;

    sf::Font m_Font;
    sf::Text m_SongTitle;

    Player m_Player;
    Spectrum m_Spectrum;

    sf::Texture m_Texture;
    std::vector<CircleButton> m_Buttons;

    /**
     * Définit les propriétés des sprites/shapes
     * (texture, positions).
    */
    virtual void loadImages();

    /**
     * Dessine l'ensemble des éléments de l'application
     * (sprites, shapes, textes et vertices).
    */
    virtual void drawWindowContent();

    /**
     * Lance le son song du player
     * et actualise l'interface graphique.
    */
    virtual void changeSong(int song);

  public:

    Interface();
    virtual ~Interface();

    /**
     * Lance la boucle principale du programme.
    */
    virtual void run();
};

#endif  // __INTERFACE_HPP__
