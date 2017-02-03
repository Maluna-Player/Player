/*************************************
 * @file    MenuBar.cpp
 * @date    14/09/15
 * @author  Manuel
 *
 * Définitions de la classe MenuBar.
 *************************************
*/

#include "MenuBar.h"
#include "Constants.h"
#include "../Util/Tools.h"


namespace gui {


MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    setStyleSheet("background-color: grey;");

    QMenu *fileMenu = addMenu("Fichier");
    QMenu *optionsMenu = addMenu("Options");
    QMenu *helpMenu = addMenu("Aide");

    mp_AddingSongAction = fileMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "adding.png")), "Ajouter un son");
    mp_OpenAction = fileMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "open.png")), "Ouvrir");
    mp_QuitAction = fileMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "quit.png")), "Quitter");
    mp_OpenConnectionAction = optionsMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "connection.png")), "Fenêtre de connexion");
    mp_AboutAction = helpMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "about.png")), "A propos");

    mp_AddingSongAction->setShortcut(QKeySequence("Ctrl+N"));
    mp_OpenAction->setShortcut(QKeySequence("Ctrl+O"));
    mp_QuitAction->setShortcut(QKeySequence("Ctrl+Q"));
    mp_AboutAction->setShortcut(QKeySequence("Ctrl+A"));
}

// ==============================
// ==============================

QAction* MenuBar::getAddingSongAction() const
{
    return mp_AddingSongAction;
}

// ==============================
// ==============================

QAction* MenuBar::getOpenAction() const
{
    return mp_OpenAction;
}

// ==============================
// ==============================

QAction* MenuBar::getQuitAction() const
{
    return mp_QuitAction;
}

// ==============================
// ==============================

QAction* MenuBar::getOpenConnectionAction() const
{
    return mp_OpenConnectionAction;
}

// ==============================
// ==============================

QAction* MenuBar::getAboutAction() const
{
    return mp_AboutAction;
}


} // gui
