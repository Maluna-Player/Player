/*************************************
 * @file    PlayerTab.hpp
 * @date    22/09/14
 * @author  Manuel
 *
 * Déclarations de la classe PlayerTab
 * correspondant à l'onglet lattéral
 * de l'application.
 *************************************
*/

#ifndef __PLAYERTAB_HPP__
#define __PLAYERTAB_HPP__

#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Clickable.hpp"

class PlayerTab : public sf::Drawable, public sf::Transformable, public Clickable
{
  private:

    enum TabState_t { CLOSED_STATE, OPENED_SATE, LEFT_MOVING_STATE, RIGHT_MOVING_STATE };

    TabState_t m_State;

    sf::Texture m_TabTexture;

    sf::Sprite m_TabHead;

    sf::RenderTexture m_BackgroundTexture;

    sf::RectangleShape m_TabBackground;

    std::vector<sf::Drawable*> mp_TabElements;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  public:

    PlayerTab();
    virtual ~PlayerTab();

    /**
     * @return Position du corps de l'onglet dans la fenêtre.
    */
    virtual const sf::Vector2f& getPosition() const;

    /**
     * @return true si l'onglet n'est pas fermé.
    */
    virtual bool isOpened() const;

    /**
     * @return true si l'onglet est en déplacement.
    */
    virtual bool isMoving() const;

    virtual bool collision(int x, int y) const;

    /**
     * Change l'état de l'onglet (ouvert/fermé/déplacement).
    */
    virtual void changeState();

    /**
     * Déplace l'onglet (tête et corps)
     * selon son état actuel.
    */
    virtual void moveTab();

    /**
     * Ajoute l'objet passé en paramètre
     * au contenu de l'onglet.
     * @param object Element à ajouter
    */
    virtual void add(sf::Drawable *object);

    /**
     * Dessine l'ensemble des éléments de l'onglet
     * sur le fond (RenderTexture).
    */
    virtual void drawContentOnBackground();
};

#endif  // __PLAYERTAB_HPP__
