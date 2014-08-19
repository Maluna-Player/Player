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
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/System/Clock.hpp>

/** SFML Graphics includes **/
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Player.hpp"
#include "Spectrum.hpp"
#include "CircleButton.hpp"
#include "ProgressBar.hpp"
#include "ProgressBackground.hpp"
#include "Input.hpp"

class Interface
{
  private:

    sf::Clock m_Clock;

    sf::RenderWindow m_Window;
    Input m_In;

    sf::Font m_Font;
    sf::Text m_SongTitle;

    Player m_Player;
    Spectrum m_Spectrum;

    std::vector<sf::Texture> m_Textures;
    std::vector<CircleButton> m_Buttons;

    ProgressBackground *mp_ProgressBackground;
    ProgressBar *mp_ProgressBar;


    /**
     * Active la fonction sleep pour que le
     * temps passé entre 2 tours de boucles
     * corresponde à ms.
     * @param ms Intervalle de temps
    */
    virtual void wait(int ms);

    /**
     * Charge les textures et créé
     * l'ensemble des sprites/shapes.
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

    /**
     * Modifie la position du son et
     * met à jour la barre de progression.
     * @param x Nouvelle position de la barre
    */
    virtual void setSongPosition(int x);

  public:

    Interface();
    virtual ~Interface();

    /**
     * Lance la boucle principale du programme.
    */
    virtual void run();
};

#endif  // __INTERFACE_HPP__
