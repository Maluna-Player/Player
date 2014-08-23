/*************************************
 * @file    main.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Fonction main du projet.
 * Lance l'application.
 *************************************
*/

#include <cstdlib> /* EXIT_SUCCESS/EXIT_FAILURE */
#include <SFML/System/Err.hpp>

#include "Application.hpp"

#include "BaseException.hpp"

int main(void)
{
  Application app;

  try
  {
    app.run();
  }
  catch (BaseException& e)
  {
    std::ostream& errorOutput = sf::err();
    errorOutput << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
