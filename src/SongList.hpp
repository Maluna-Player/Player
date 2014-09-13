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
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "Clickable.hpp"

class SongList : public sf::Drawable, public sf::Transformable, public Clickable
{
  private:

    sf::Font m_Font;

    std::vector<sf::Text> m_Titles;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  public:

    SongList();
    virtual ~SongList();

    virtual bool collision(int x, int y) const;

    /**
     * Ajoute le titre passé en paramètre à la liste.
     * @param title Titre à ajouter
    */
    virtual void add(const std::string& title);

    /**
     * Ajoute les titres de la liste passée en paramètre.
     * @param titles Liste de titres à ajouter
    */
    virtual void add(const std::vector<std::string>& titles);

    /**
     * Efface la liste des titres affichés.
    */
    virtual void clear();
};

#endif  // __SONGLIST_HPP__
