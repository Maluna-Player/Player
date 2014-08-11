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

#include "Fmod.hpp"

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
     * @param id Identifiant du canal à tester
    */
    virtual void update(SoundID_t id);
};

#endif  // __SPECTRUM_HPP__
