/*************************************
 * @file    Application.hpp
 * @date    20/08/14
 * @author  Manuel
 *
 * Déclarations de la classe Application
 * contenant le lancement de l'application
 * et les gestionnaires graphique
 * et audio.
 *************************************
*/

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

/** SFML Window includes **/
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/System/Clock.hpp>

#include "Input.hpp"
#include "Player.hpp"
#include "Interface.hpp"

class Application
{
  private:

    sf::RenderWindow m_Window;
    Input m_In;

    sf::Clock m_Clock;

    Player m_Player;
    Interface m_Interface;


    /**
     * Active la fonction sleep pour que le
     * temps passé entre 2 tours de boucles
     * corresponde à l'intervalle ms.
     * @param ms Intervalle de temps
    */
    virtual void wait(int ms);

    /**
     * Change l'état du player
     * et modifie l'affichage.
    */
    virtual void setState(State_t state);

    /**
     * Lance le son song du player
     * et actualise l'interface graphique
     * (titre, durée...)
    */
    virtual void changeSong(int song);

    /**
     * Modifie la position du son et
     * met à jour la barre de progression.
     * @param x Nouvelle position de la barre
    */
    virtual void setSongPosition(int x);

    /**
     * Modifie le volume du son joué.
     * @param volumeType Modification (plus ou moins)
    */
    virtual void setVolume(int volumeType);

  public:

    Application();
    virtual ~Application();

    /**
     * Lance la boucle principale du programme.
    */
    virtual void run();

};

#endif  // __APPLICATION_HPP__
