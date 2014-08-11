/*************************************
 * @file    CircleButton.cpp
 * @date    11/08/14
 * @author  Manuel
 *
 * Définitions de la classe CircleButton.
 *************************************
*/

#include "CircleButton.hpp"


CircleButton::CircleButton(float radius, unsigned int pointCount)
  : sf::CircleShape(radius, pointCount)
{

}

// ==============================
// ==============================

CircleButton::~CircleButton()
{

}

// ==============================
// ==============================

bool CircleButton::collision(int x, int y) const
{
  sf::Vector2f point(x, y);
  sf::FloatRect boundingBox = getGlobalBounds();

  return (boundingBox.contains(point));
}
