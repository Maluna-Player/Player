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
#include "PlayerTab.hpp"


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
    PlayerTab *mp_Tab;

    std::vector<Clickable*> mp_ClickableObjects;
    std::vector<Movable*>   mp_MovableObjects;


    /**
     * Convertit le temps passé en secondes
     * en une chaîne de caractères min:sec.
     * @param seconds Nombre de secondes
     * @return Chaîne formatée
    */
    virtual const std::string timeToString(int seconds) const;

    /**
     * Convertit des coordonnées de la fenêtre
     * en coordonnées relatives à l'onglet.
     * @param pos Coordonnées d'un point la fenêtre
     * @return Coordonnées dans l'onglet
    */
    virtual const sf::Vector2f getTabRelativePosition(const sf::Vector2f& pos) const;

  public:

    Interface();
    virtual ~Interface();

    virtual SongList& getSongList();

    virtual Movable& button(Movable_t index) const;

    /**
     * Détermine s'il y a collision entre l'objet et les
     * coordonnées passés en paramètre.
     * @param index Objet cliquable à tester
     * @param x Abscisse du point
     * @param y Ordonnée du point
     * @return true si collision, false sinon
    */
    virtual bool collision(Clickable_t index, int x, int y) const;

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

    /**
     * Change l'état de l'onglet (ouvert/fermé/déplacement).
    */
    virtual void changeTabState();

    /**
     * Déplace l'onglet s'il est en déplacement.
    */
    virtual void moveTab();

    /**
     * Détermine le son pointé/cliqué par
     * le point de coordonnées (x,y).
     * @param x Abscisse du point
     * @param x Ordonnée du point
     * @return Numéro de la chanson, -1 si aucune
    */
    virtual int getSongInList(int x, int y) const;
};

#endif  // __INTERFACE_HPP__
