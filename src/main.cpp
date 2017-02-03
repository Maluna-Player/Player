/*************************************
 * @file    main.cpp
 * @date    26/04/15
 * @author  Manuel
 *
 * Fonction main du projet.
 * Lance l'application.
 *************************************
*/

#include "Gui/PlayerWindow.h"
#include "Exceptions/BaseException.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    try
    {
        QApplication qapp(argc, argv);
        gui::PlayerWindow window;
        window.show();

        return qapp.exec();
    }
    catch (exceptions::BaseException& e)
    {
        qDebug() << e.what();
        return EXIT_FAILURE;
    }
}
