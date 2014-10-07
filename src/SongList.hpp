/*************************************
 * @file    SongList.hpp
 * @date    12/09/14
 * @author  Manuel
 *
 * Déclarations de la classe SongList
 * comprenant l'affichage de la liste
 * des sons ouverts dans le player.
 *************************************
*/

#ifndef __SONGLIST_HPP__
#define __SONGLIST_HPP__

#include <vector>
#include <utility>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Clickable.hpp"

class SongList : public sf::Drawable, public sf::Transformable, public Clickable
{
  private:

    typedef std::vector<std::pair<sf::Text, sf::Text> >   SongTexts_t;


    int m_Selected;
    int m_Pointed;

    sf::Font m_Font;

    sf::Texture m_PointingTexture;
    sf::RectangleShape m_PointingBox;

    SongTexts_t m_SongDetails;

    int m_Scroll;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  public:

    SongList();
    virtual ~SongList();

    virtual bool collision(int x, int y) const;

    /**
     * Détermine la chanson située aux coordonnées
     * (x,y), qui soit pointée ou cliquée.
     * @param x Abscisse du point
     * @param y Ordonnée du point
     * @return Numéro de la chanson, -1 si aucune
    */
    virtual int getSong(int x, int y) const;

    /**
     * Change le son marqué comme son courant
     * dans la liste.
     * @param song Indice de la musique actuelle
    */
    virtual void setCurrentSong(int song);

    /**
     * Change le son marqué comme pointé
     * par l'utilisateur dans la liste.
     * @param song Indice de la musique
    */
    virtual void setPointedSong(int song);

    /**
     * Ajoute le titre et la durée passés en paramètre.
     * @param title Titre à ajouter
     * @param length Durée à ajouter
    */
    virtual void add(const std::string& title, const std::string& length);

    /**
     * Ajoute les titres et durées de la liste passée en paramètre.
     * @param songs Liste de sons (titre/durée) à ajouter
    */
    virtual void add(const std::vector<std::pair<std::string, std::string> >& songs);

    /**
     * Efface la liste des titres affichés.
    */
    virtual void clear();

    /**
     * Fait défiler la liste et modifie
     * l'affichage des titres visibles.
     * @param delta Nombre de crans de scroll
    */
    virtual void scroll(int delta);
};

#endif  // __SONGLIST_HPP__
