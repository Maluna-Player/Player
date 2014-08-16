/*************************************
 * @file    ProgressBar.cpp
 * @date    15/08/14
 * @author  Manuel
 *
 * Définitions de la classe ProgressBar.
 *************************************
*/

#include "ProgressBar.hpp"
#include "constants.hpp"
#include "FileLoadingException.hpp"


ProgressBar::ProgressBar()
{
  std::string barTexturePath = std::string(IMAGES_SUBDIR) + "/progressBar.png";
  std::string markerTexturePath = std::string(IMAGES_SUBDIR) + "/marker.png";

  if (!m_BarTexture.loadFromFile(barTexturePath))
    throw FileLoadingException("ProgressBar::ProgressBar", barTexturePath);

  if (!m_MarkerTexture.loadFromFile(markerTexturePath))
    throw FileLoadingException("ProgressBar::ProgressBar", markerTexturePath);

  m_BarTexture.setRepeated(true);

  m_Bar.setTexture(&m_BarTexture);
  m_Bar.setPosition(sf::Vector2f(PROGRESS_BACKGROUND_X, PROGRESSBAR_Y));

  m_Marker.setTexture(m_MarkerTexture);
}

// ==============================
// ==============================

ProgressBar::~ProgressBar()
{

}

// ==============================
// ==============================

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  // on applique la transformation
  states.transform *= getTransform();

  // on applique la texture du tileset
  states.texture = 0;

  // on dessine la barre et le marqueur
  target.draw(m_Bar, states);
  target.draw(m_Marker, states);
}

// ==============================
// ==============================

void ProgressBar::resize(unsigned int width)
{
  if (width > (PROGRESS_BACKGROUND_X + WINDOW_WIDTH))
    width = PROGRESS_BACKGROUND_X + WINDOW_WIDTH;

  m_Bar.setSize(sf::Vector2f(width, PROGRESSBAR_HEIGHT));
  m_Marker.setPosition(sf::Vector2f(width - 2, PROGRESSBAR_Y - 3));
}
