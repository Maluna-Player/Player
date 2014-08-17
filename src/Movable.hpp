/*************************************
 * @file    Movable.hpp
 * @date    17/08/14
 * @author  Manuel
 *
 * Déclarations de l'interface Movable
 * contenant les propriétés d'un objet
 * déplaçable.
 *************************************
*/

#ifndef __MOVABLE_HPP__
#define __MOVABLE_HPP__

#include "Clickable.hpp"

class Movable : public Clickable
{
  public:

    bool m_Press;

    virtual ~Movable() {}

    /**
     * Déplace l'objet au point de
     * coordonnées (x,y).
     * @param x Abscisse du point
     * @param y Ordonnée du point
    */
    virtual void move(int x, int y) = 0;

};

#endif  // __MOVABLE_HPP__
