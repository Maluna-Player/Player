/*************************************
 * @file    ProgressBar.hpp
 * @date    15/08/14
 * @author  Manuel
 *
 * Déclarations de la classe ProgressBar
 * contenant les propriétés de la barre
 * de progression du lecteur.
 *************************************
*/

#ifndef __PROGRESSBAR_HPP__
#define __PROGRESSBAR_HPP__

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

class ProgressBar : public sf::Drawable, public sf::Transformable
{
  private:

    sf::Texture m_BarTexture;
    sf::Texture m_MarkerTexture;

    sf::RectangleShape m_Bar;
    sf::Sprite m_Marker;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  public:

    ProgressBar();
    virtual ~ProgressBar();

    /**
     * Redimensionne la barre de progression
     * et déplace le marqueur.
     * @param width Nouvelle largeur.
    */
    virtual void resize(unsigned int width);

};

#endif  // __PROGRESSBAR_HPP__
