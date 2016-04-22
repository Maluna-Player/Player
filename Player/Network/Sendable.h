/*************************************
 * @file    Sendable.h
 * @date    02/07/15
 * @author  Manuel
 *
 * Déclarations de la classe Sendable
 * définissant l'interface d'objets
 * transférables entre deux clients.
 *************************************
*/

#ifndef __SENDABLE_H__
#define __SENDABLE_H__

#include <QByteArray>


namespace network {


class Sendable
{
    public:

        /**
         * @brief Construit le paquet à envoyer avec le contenu de l'objet.
         * @return Paquet contenant les informations de l'objet
         */
        virtual QByteArray toPacket() const = 0;
};


} // network

#endif  // __SENDABLE_H__
