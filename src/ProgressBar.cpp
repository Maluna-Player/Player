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
  if (!m_BarTexture.loadFromFile(std::string(IMAGES_SUBDIR) + "/progressBar.png"))
    throw FileLoadingException("ProgressBar::ProgressBar", std::string(IMAGES_SUBDIR) + "/progressBar.png");

  if (!m_MarkerTexture.loadFromFile(std::string(IMAGES_SUBDIR) + "/marker.png"))
    throw FileLoadingException("ProgressBar::ProgressBar", std::string(IMAGES_SUBDIR) + "/marker.png");

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

void ProgressBar::update(unsigned int pos, unsigned int totalLength)
{
  int barWidth = pos * WINDOW_WIDTH / totalLength;

  m_Bar.setSize(sf::Vector2f(barWidth, PROGRESSBAR_HEIGHT));
  m_Marker.setPosition(sf::Vector2f(barWidth - 2, PROGRESSBAR_Y - 3));
}
