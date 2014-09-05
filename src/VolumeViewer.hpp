/*************************************
 * @file    VolumeViewer.hpp
 * @date    03/09/14
 * @author  Manuel
 *
 * Déclarations de la classe VolumeViewer
 * correspondant à l'afficheur du volume.
 *************************************
*/

#ifndef __VOLUMEVIEWER_HPP__
#define __VOLUMEVIEWER_HPP__

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Clickable.hpp"

class VolumeViewer : public sf::Sprite, public Clickable
{
  private:

    sf::Texture m_VolumeTexture;

  public:

    VolumeViewer();
    virtual ~VolumeViewer();

    virtual bool collision(int x, int y) const;
};

#endif  // __VOLUMEVIEWER_HPP__
