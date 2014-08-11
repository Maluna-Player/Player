/*************************************
 * @file    CircleButton.hpp
 * @date    11/08/14
 * @author  Manuel
 *
 * Déclarations de la classe CircleButton
 * correspondant à un bouton cliquable.
 *************************************
*/

#ifndef __CIRCLEBUTTON_HPP__
#define __CIRCLEBUTTON_HPP__

#include <SFML/Graphics/CircleShape.hpp>


class CircleButton : public sf::CircleShape
{
  public:

    CircleButton(float radius = 0, unsigned int pointCount = 30);
    virtual ~CircleButton();

    virtual bool collision(int x, int y) const;
};

#endif  // __CIRCLEBUTTON_HPP__
