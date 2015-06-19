/*************************************
 * @file    ProgressBackground.h
 * @date    16/08/14
 * @author  Manuel
 *
 * Déclarations de la classe
 * ProgressBackground correspondant au
 * fond de la barre de progression.
 *************************************
*/

#ifndef __PROGRESSBACKGROUND_H__
#define __PROGRESSBACKGROUND_H__

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

class ProgressBackground : public QWidget
{
    protected:

        virtual void paintEvent(QPaintEvent *event);

    public:

        ProgressBackground(QWidget *parent = 0);
        virtual ~ProgressBackground();
};

#endif  // __PROGRESSBACKGROUND_H__
