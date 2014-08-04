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

#include <vector>
#include <SFML/Graphics/VertexArray.hpp>

class Spectrum
{
  private:

    std::vector<sf::VertexArray> m_Vertices;

  public:

    Spectrum();
    virtual ~Spectrum();

    /**
     * @param i indice du VertexArray à retourner
     * @return VertexArray à l'indice i
    */
    virtual const sf::VertexArray& getLine(unsigned int i) const;

    /**
     * Récupère les fréquences du son joué
     * et met à jour les valeurs des vertices.
    */
    virtual void update();
};

#endif  // __SPECTRUM_HPP__
