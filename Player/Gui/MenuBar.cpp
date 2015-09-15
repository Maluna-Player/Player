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
#include "Tools.h"


MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    setStyleSheet("background-color: grey;");

    QMenu *fileMenu = addMenu("Fichier");
    QMenu *helpMenu = addMenu("Aide");

    mp_OpenAction = fileMenu->addAction(QIcon(Tools::loadImage(MENU_SUBDIR + "open.png")), "Open");
    mp_QuitAction = fileMenu->addAction(QIcon(Tools::loadImage(MENU_SUBDIR + "quit.png")), "Quitter");
    mp_AboutAction = helpMenu->addAction(QIcon(Tools::loadImage(MENU_SUBDIR + "about.png")), "A propos");

    mp_OpenAction->setShortcut(QKeySequence("Ctrl+O"));
    mp_QuitAction->setShortcut(QKeySequence("Ctrl+Q"));
    mp_AboutAction->setShortcut(QKeySequence("Ctrl+A"));
}

// ==============================
// ==============================

MenuBar::~MenuBar()
{

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
// ==============================1

QAction* MenuBar::getAboutAction() const
{
    return mp_AboutAction;
}
