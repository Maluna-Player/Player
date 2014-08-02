/*************************************
 * @file    main.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Fonction main du projet.
 * Lance l'application.
 *************************************
*/

#include <cstdlib> /* EXIT_SUCCESS */

#include "Interface.hpp"

int main(void)
{
  Interface player;
  player.run();

  return EXIT_SUCCESS;
}
