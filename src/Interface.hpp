/*************************************
 * @file    Interface.hpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Déclarations de la classe Interface
 * contenant la gestion de l'interface
 * de l'application.
 *************************************
*/

#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

/** SFML Graphics includes **/
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Spectrum.hpp"
#include "CircleButton.hpp"
#include "ProgressBar.hpp"
#include "ProgressBackground.hpp"
#include "VolumeViewer.hpp"


class Interface
{
  private:

    sf::Font m_Font;
    sf::Text m_SongTitle;
    sf::Text m_SongPos;

    Spectrum m_Spectrum;

    std::vector<sf::Texture> m_Textures;
    std::vector<CircleButton> m_Buttons;

    ProgressBackground *mp_ProgressBackground;
    ProgressBar *mp_ProgressBar;
    VolumeViewer *mp_VolumeViewer;

    std::vector<Clickable*> mp_ClickableObjects;
    std::vector<Movable*>   mp_MovableObjects;


    /**
     * Convertit le temps passé en secondes
     * en une chaîne de caractères min:sec.
     * @param seconds Nombre de secondes
     * @return Chaîne formatée
    */
    const std::string timeToString(int seconds) const;

  public:

    Interface();
    virtual ~Interface();

    virtual Clickable& button(Clickable_t index) const;

    virtual Movable& button(Movable_t index) const;

    /**
     * Charge les polices et créé
     * l'ensemble des textes.
    */
    virtual void loadTexts();

    /**
     * Charge les textures et créé
     * l'ensemble des sprites/shapes.
    */
    virtual void loadImages();

    /**
     * Dessine l'ensemble des éléments de l'application
     * (sprites, shapes, textes et vertices).
     * @param target Surface sur laquelle afficher le contenu
     * @param stopped Etat stop du player
    */
    virtual void drawContent(sf::RenderTarget& target, bool stopped);

    /**
     * Change le titre du son courant.
     * @param title Titre à afficher
    */
    virtual void setTitle(const std::string& title);

    /**
     * Modifie la texture du bouton
     * play/pause.
     * @param play Etat du player
    */
    virtual void setPlayButtonTexture(bool play);

    /**
     * Modifie la texture du volume.
     * @param volumeState Etat du volume du player
    */
    virtual void setVolumeTexture(int volumeState);

    /**
     * Modifie la taille de la barre de
     * progression.
     * @param pos Position de la chanson
     * @param length Durée de la chanson
    */
    virtual void setSoundPosition(SoundPos_t pos, SoundPos_t length);

    /**
     * Actualise le spectre du son.
     * @param id Identifiant du canal
    */
    virtual void updateSpectrum(SoundID_t id);
};

#endif  // __INTERFACE_HPP__
