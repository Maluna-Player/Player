/*************************************
 * @file    Path.cpp
 * @date    01/08/14
 * @author  Manuel
 *
 * Définitions de la classe Path.
 *************************************
*/

#include "Path.hpp"

#include <cstddef>


std::string Path::baseName(const std::string& filePath)
{
  /* Recherche du dernier séparateur */
  std::size_t found = filePath.find_last_of("/\\");

  /* Récupération du nom de fichier */
  std::string file = filePath.substr(found+1);

  /* Recherche du dernier '.' */
  found = file.find_last_of(".");

  return file.substr(0, found);
}

// ==============================
// ==============================

char Path::separator()
{
  #ifdef _WIN32
    return '\\';
  #else
    return '/';
  #endif
}
