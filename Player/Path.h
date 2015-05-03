/*************************************
 * @file    Path.h
 * @date    01/08/14
 * @author  Manuel
 *
 * Déclarations de la classe Path
 * contenant des méthodes statiques
 * de gestion de chemins.
 *************************************
*/

#ifndef __PATH_H__
#define __PATH_H__

#include <string>

class Path
{
  public:

    /**
     * Récupère le nom du fichier sans extension à partir d'un chemin.
     * @param filePath Chemin du fichier
     * @return nom du fichier
    */
    static std::string baseName(const std::string& filePath);

    /**
     * @return Caractère séparateur des chemins.
    */
    static char separator();
};

#endif  // __PATH_H__
