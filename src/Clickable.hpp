/*************************************
 * @file    Clickable.hpp
 * @date    16/08/14
 * @author  Manuel
 *
 * Déclarations de l'interface Clickable
 * contenant les propriétés d'un objet
 * cliquable.
 *************************************
*/

#ifndef __CLICKABLE_HPP__
#define __CLICKABLE_HPP__

class Clickable
{
  public:

    virtual ~Clickable() {}

    /**
     * Détermine si le point (x,y) est contenu
     * dans l'objet.
     * @param x Abscisse du point
     * @param y Ordonnée du point
     * @return true si collision, sinon false
    */
    virtual bool collision(int x, int y) const = 0;

};

#endif  // __CLICKABLE_HPP__
