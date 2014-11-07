/*************************************
 * @file    Input.cpp
 * @date    18/08/14
 * @author  Manuel
 *
 * Définitions de la classe Input.
 *************************************
*/

#include "Input.hpp"
#include <SFML/Window/Event.hpp>


Input::Input()
{
  int i;

  for (i = 0; i < sf::Keyboard::KeyCount; i++)
    m_Keys[i] = false;
}

// ==============================
// ==============================

Input::~Input()
{

}

// ==============================
// ==============================

bool Input::keyPressed(int keyCode)
{
  if (m_Keys[keyCode])
  {
    m_Keys[keyCode] = false;
    return true;
  }

  return false;
}

// ==============================
// ==============================

void Input::update(sf::Window& window)
{
  sf::Event event;

  m_Motion = false;
  m_Clic = false;
  m_Unclick = false;
  m_WheelDelta = 0;

  while (window.pollEvent(event))
  {
    switch (event.type)
    {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::MouseMoved:
        m_Motion = true;
        break;

      case sf::Event::MouseButtonPressed:
        m_Clic = true;
        m_ButtonX = event.mouseButton.x;
        m_ButtonY = event.mouseButton.y;
        break;

      case sf::Event::MouseButtonReleased:
        m_Unclick = true;
        m_ButtonX = event.mouseButton.x;
        m_ButtonY = event.mouseButton.y;
        break;

      case sf::Event::MouseWheelMoved:
        m_WheelDelta = event.mouseWheel.delta;
        break;

      case sf::Event::KeyPressed:
        m_Keys[event.key.code] = true;
        break;

      case sf::Event::KeyReleased:
        m_Keys[event.key.code] = false;
        break;

      default:
        break;
    }
  }
}
