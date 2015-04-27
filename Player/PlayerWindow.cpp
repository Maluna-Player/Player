/*************************************
 * @file    PlayerWindow.cpp
 * @date    26/04/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerWindow.
 *************************************
*/

#include "PlayerWindow.h"
#include "FmodManager.h"

PlayerWindow::PlayerWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr(WINDOW_TITLE));
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

PlayerWindow::~PlayerWindow()
{
    FmodManager::deleteInstance();
}
