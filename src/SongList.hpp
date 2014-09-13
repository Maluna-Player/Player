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
#include "Clickable.hpp"

class SongList : public sf::Drawable, public sf::Transformable, public Clickable
{
  private:

    typedef std::vector<std::pair<sf::Text, sf::Text> >   SongTexts_t;

    sf::Font m_Font;

    SongTexts_t m_SongDetails;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  public:

    SongList();
    virtual ~SongList();

    virtual bool collision(int x, int y) const;

    /**
     * Détermine la chanson cliquée
     * avec le point (x,y).
     * @param x Abscisse du point
     * @param y Ordonnée du point
     * @return Numéro de la chanson, -1 si aucune
    */
    virtual int getClickedSong(int x, int y) const;

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
};

#endif  // __SONGLIST_HPP__
