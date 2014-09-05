/*************************************
 * @file    ProgressBackground.cpp
 * @date    16/08/14
 * @author  Manuel
 *
 * Définitions de la classe ProgressBackground.
 *************************************
*/

#include "ProgressBackground.hpp"
#include "constants.hpp"
#include "FileLoadingException.hpp"
#include "Path.hpp"


ProgressBackground::ProgressBackground(const sf::Vector2f& size)
  : sf::RectangleShape(size)
{
  std::string backgroundTexturePath = std::string(IMAGES_SUBDIR) + Path::separator() + "progressBackground.png";

  if (!m_BackgroundTexture.loadFromFile(backgroundTexturePath))
    throw FileLoadingException("ProgressBar::ProgressBar", backgroundTexturePath);

  m_BackgroundTexture.setRepeated(true);

  setTexture(&m_BackgroundTexture);
  setPosition(sf::Vector2f(PROGRESS_BACKGROUND_X, PROGRESS_BACKGROUND_Y));
}

// ==============================
// ==============================

ProgressBackground::~ProgressBackground()
{

}

// ==============================
// ==============================

bool ProgressBackground::collision(int x, int y) const
{
  return (x >= PROGRESS_BACKGROUND_X && x <= (PROGRESS_BACKGROUND_X + WINDOW_WIDTH)
            && y >= PROGRESSBAR_Y && y <= (PROGRESSBAR_Y + PROGRESSBAR_HEIGHT));
}
