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

#include <vector>
#include <utility>

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
#include "SongList.hpp"


class Interface
{
  private:

    sf::Font m_Font;
    std::vector<sf::Text> m_Texts;

    Spectrum m_Spectrum;

    std::vector<sf::Texture> m_Textures;
    std::vector<CircleButton> m_Buttons;

    ProgressBackground *mp_ProgressBackground;
    ProgressBar *mp_ProgressBar;
    VolumeViewer *mp_VolumeViewer;
    SongList *mp_SongList;

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

    virtual SongList& getSongList();

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
     * Change le titre et la durée du son courant.
     * @param num Numéro de la musique dans la liste
     * @param title Titre à afficher
     * @param length Durée du son
    */
    virtual void setSong(int num, const std::string& title, SoundPos_t length);

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

    /**
     * Ajoute les infos de la liste passée en paramètre
     * à la SongList en formatant les données
     * (longueur du titre et type de la durée).
     * @param songs Liste de sons (titre/durée) à ajouter
    */
    virtual void addToSongList(const std::vector<std::pair<std::string, int> >& songs);
};

#endif  // __INTERFACE_HPP__
