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
    setStyleSheet("background-color: rgb(195, 195, 195);");

    QMenu *fileMenu = addMenu("Fichier");
    QMenu *optionsMenu = addMenu("Options");
    QMenu *helpMenu = addMenu("Aide");

    // Menu "Fichier"
    mp_AddingSongAction = fileMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "adding.png")), "Ajouter un son");
    mp_OpenAction = fileMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "open.png")), "Ouvrir");
    mp_QuitAction = fileMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "quit.png")), "Quitter");

    // Menu "Options"
    mp_OpenConnectionAction = optionsMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "connection.png")), "Fenêtre de connexion");
    mp_ChangeSpectrumColorAction = optionsMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "color.png")), "Couleurs du spectre");
    mp_ProfileAction = optionsMenu->addAction(QIcon(util::Tools::loadImage(QString(MENU_SUBDIR) + "profile.png")), "Profil");

    // Menu "Aide"
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

QAction* MenuBar::getChangeSpectrumColorAction() const
{
    return mp_ChangeSpectrumColorAction;
}

// ==============================
// ==============================

QAction* MenuBar::getProfileAction() const
{
    return mp_ProfileAction;
}

// ==============================
// ==============================

QAction* MenuBar::getAboutAction() const
{
    return mp_AboutAction;
}


} // gui
