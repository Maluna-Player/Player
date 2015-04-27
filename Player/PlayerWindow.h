/*************************************
 * @file    PlayerWindow.hpp
 * @date    26/04/15
 * @author  Manuel
 *
 * Déclarations de la classe PlayerWindow
 * correspondant à la fenêtre principale.
 *************************************
*/

#ifndef __PLAYERWINDOW_H__
#define __PLAYERWINDOW_H__

#include <QWidget>

class PlayerWindow : public QWidget
{
    Q_OBJECT

    public:

        PlayerWindow(QWidget *parent = 0);
        virtual ~PlayerWindow();
};

#endif // __PLAYERWINDOW_H__
