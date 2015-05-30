/*************************************
 * @file    main.cpp
 * @date    26/04/15
 * @author  Manuel
 *
 * Fonction main du projet.
 * Lance l'application.
 *************************************
*/

#include "PlayerWindow.h"
#include "BaseException.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    try
    {
        QApplication qapp(argc, argv);
        PlayerWindow window;
        window.show();

        return qapp.exec();
    }
    catch (BaseException& e)
    {
        qDebug() << e.what();
        return EXIT_FAILURE;
    }
}
