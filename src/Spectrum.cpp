/*************************************
 * @file    Spectrum.cpp
 * @date    03/08/14
 * @author  Manuel
 *
 * Définitions de la classe Spectrum.
 *************************************
*/

#include "Spectrum.hpp"
#include "constants.hpp"
#include "Fmod.hpp"
#include "ArrayAccessException.hpp"

#include <cmath>


Spectrum::Spectrum() : m_Vertices(SPECTRUM_WIDTH, sf::VertexArray(sf::Lines, 2))
{
  int i;

  for (i = 0; i < SPECTRUM_WIDTH; i++)
  {
    m_Vertices[i][1].position = sf::Vector2f(SPECTRUM_X + i, SPECTRUM_Y + SPECTRUM_HEIGHT);
    m_Vertices[i][1].color = sf::Color::Green;
  }
}

// ==============================
// ==============================

Spectrum::~Spectrum()
{

}

// ==============================
// ==============================

const sf::VertexArray& Spectrum::getLine(unsigned int i) const
{
  if (i > SPECTRUM_WIDTH)
    throw ArrayAccesException("Spectrum::getLine");

  return m_Vertices.at(i);
}

// ==============================
// ==============================

void Spectrum::update()
{
  int i;
  float spectrumValues[SPECTRUM_WIDTH] = {0.0};

  /* Récupération des valeurs spectrales de FMOD */
  Fmod::getInstance()->getChannelSpectrum(0, spectrumValues);

  for (i = 0; i < SPECTRUM_WIDTH; i++)
  {
    int lineHeight = spectrumValues[i] * SPECTRUM_HEIGHT * SPECTRUM_RATIO;

    if (lineHeight > SPECTRUM_HEIGHT)
      lineHeight = SPECTRUM_HEIGHT;
    else if (lineHeight < 0)
      lineHeight = 0;

    int yPos = SPECTRUM_HEIGHT - lineHeight;

    /* Calcul de la position de la colonne */
    m_Vertices[i][0].position = sf::Vector2f(SPECTRUM_X + i, SPECTRUM_Y + yPos);

    /* Calcul des couleurs de l'extrémité supérieure de la colonne */
    int r = fabs((yPos * 255) / SPECTRUM_HEIGHT - 255);
    int g = fabs((yPos * 255) / SPECTRUM_HEIGHT - 0);
    int b = fabs((yPos * 0) / SPECTRUM_HEIGHT - 0);
    m_Vertices[i][0].color = sf::Color(r, g, b);
  }
}
