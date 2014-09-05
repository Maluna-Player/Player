/*************************************
 * @file    VolumeViewer.cpp
 * @date    03/09/14
 * @author  Manuel
 *
 * Définitions de la classe VolumeViewer.
 *************************************
*/

#include "VolumeViewer.hpp"
#include "constants.hpp"
#include "FileLoadingException.hpp"
#include "Path.hpp"


VolumeViewer::VolumeViewer()
{
  std::string volumeTexturePath = std::string(IMAGES_SUBDIR) + Path::separator() + "volume.png";

  if (!m_VolumeTexture.loadFromFile(volumeTexturePath))
    throw FileLoadingException("VolumeViewer::VolumeViewer", volumeTexturePath);

  setTexture(m_VolumeTexture);
  setTextureRect(sf::IntRect(VOLUME_SPRITE_W * (NB_VOLUME_STATES - 1), 0, VOLUME_SPRITE_W, VOLUME_SPRITE_H));
  setPosition(sf::Vector2f(VOLUME_X, VOLUME_Y));
}

// ==============================
// ==============================

VolumeViewer::~VolumeViewer()
{

}

// ==============================
// ==============================

bool VolumeViewer::collision(int x, int y) const
{
  return getGlobalBounds().contains(sf::Vector2f(x, y));
}
