/*************************************
 * @file    ProgressBackground.hpp
 * @date    16/08/14
 * @author  Manuel
 *
 * Déclarations de la classe
 * ProgressBackground correspondant au
 * fond de la barre de progression.
 *************************************
*/

#ifndef __PROGRESSBACKGROUND_HPP__
#define __PROGRESSBACKGROUND_HPP__

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Clickable.hpp"

class ProgressBackground : public sf::RectangleShape, public Clickable
{
  private:

    sf::Texture m_BackgroundTexture;

  public:

    ProgressBackground(const sf::Vector2f& size = sf::Vector2f(0, 0));
    virtual ~ProgressBackground();

    virtual bool collision(int x, int y) const;
};

#endif  // __PROGRESSBACKGROUND_HPP__
