/*************************************
 * @file    Tools.h
 * @date    11/06/15
 * @author  Manuel
 *
 * Déclarations de la classe Tools contenant
 * des fonctionnalités annexes.
 *************************************
*/

#ifndef __TOOLS_H_
#define __TOOLS_H__

#include <QString>

class Tools
{
    public:

        /**
         * @brief Convertit le temps passé en ms dans le format mm:ss ou hh:mm:ss.
         * @param ms Temps à afficher en ms
         */
        static QString msToString(int ms);
};

#endif  // __TOOLS_H__
