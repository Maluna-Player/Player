/*************************************
 * @file    Input.hpp
 * @date    18/08/14
 * @author  Manuel
 *
 * Déclarations de la classe Input
 * contenant la gestion des événements
 * SFML.
 *************************************
*/

#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

class Input
{
  private:

    bool m_Motion;
    bool m_Clic;
    bool m_Unclick;
    bool m_Keys[sf::Keyboard::KeyCount];

    int m_ButtonX;
    int m_ButtonY;

    int m_WheelDelta;

  public:

    Input();
    virtual ~Input();

    inline bool motion() const    {return m_Motion;}
    inline bool clic() const      {return m_Clic;}
    inline bool unclic() const    {return m_Unclick;}
    inline int buttonX() const    {return m_ButtonX;}
    inline int buttonY() const    {return m_ButtonY;}
    inline int wheelDelta() const {return m_WheelDelta;}

    /**
     * Teste si la touche est pressée et remet
     * à faux la touche correspondante.
     * @param keyCode Touche testée
     * @return true si le bouton est pressé.
    */
    virtual bool keyPressed(int keyCode);

    /**
     * Récupère les événements SFML
     * et met à jour les états des entrées.
    */
    virtual void update(sf::Window& window);
};

#endif // __INPUT_HPP__
