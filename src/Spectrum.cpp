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

#include <cmath>

Spectrum::Spectrum() : m_Vertices(sf::Lines, SPECTRUM_WIDTH * 2)
{
  int i;

  for (i = 0; i < SPECTRUM_WIDTH; i++)
  {
    sf::Vertex* line = &m_Vertices[i * 2];

    line[0].position = sf::Vector2f(SPECTRUM_X + i, SPECTRUM_Y + SPECTRUM_HEIGHT);
    line[1].position = sf::Vector2f(SPECTRUM_X + i, SPECTRUM_Y + SPECTRUM_HEIGHT);

    line[1].color = sf::Color(255, 255, 0);
  }
}

// ==============================
// ==============================

Spectrum::~Spectrum()
{

}

// ==============================
// ==============================

void Spectrum::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  // on applique la transformation
  states.transform *= getTransform();

  // on applique la texture du tileset
  states.texture = 0;

  // et on dessine enfin le tableau de vertex
  target.draw(m_Vertices, states);
}

// ==============================
// ==============================

void Spectrum::update(SoundID_t id)
{
  int i;
  float spectrumValues[SPECTRUM_WIDTH] = {0.0};

  /* Récupération des valeurs spectrales de FMOD */
  Fmod::getInstance()->getChannelSpectrum(id, spectrumValues);

  for (i = 0; i < SPECTRUM_WIDTH; i++)
  {
    int lineHeight = spectrumValues[i] * SPECTRUM_HEIGHT * SPECTRUM_RATIO;

    if (lineHeight > SPECTRUM_HEIGHT)
      lineHeight = SPECTRUM_HEIGHT;
    else if (lineHeight < 0)
      lineHeight = 0;

    int yPos = SPECTRUM_HEIGHT - lineHeight;


    sf::Vertex* line = &m_Vertices[i * 2];

    /* Calcul de la position de la colonne */
    line[0].position = sf::Vector2f(SPECTRUM_X + i, SPECTRUM_Y + yPos);

    /* Calcul des couleurs de l'extrémité supérieure de la colonne */
    int r = fabs((yPos * 0) / SPECTRUM_HEIGHT - 255);
    int g = fabs((yPos * 255) / SPECTRUM_HEIGHT - 0);
    int b = fabs((yPos * 0) / SPECTRUM_HEIGHT - 0);
    line[0].color = sf::Color(r, g, b);
  }
}
