/*************************************
 * @file    Spectrum.hpp
 * @date    03/08/14
 * @author  Manuel
 *
 * Déclarations de la classe Spectrum
 * contenant les propriétés du spectre
 * du son.
 *************************************
*/


#ifndef __SPECTRUM_HPP__
#define __SPECTRUM_HPP__

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class Spectrum : public sf::Drawable, public sf::Transformable
{
  private:

    sf::VertexArray m_Vertices;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  public:

    Spectrum();
    virtual ~Spectrum();

    /**
     * Récupère les fréquences du son joué
     * et met à jour les valeurs des vertices.
    */
    virtual void update();
};

#endif  // __SPECTRUM_HPP__
