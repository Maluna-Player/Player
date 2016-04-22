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


namespace gui {


class ProgressBackground : public QWidget
{
    protected:

        virtual void paintEvent(QPaintEvent *event) override;

    public:

        ProgressBackground(QWidget *parent = nullptr);
        virtual ~ProgressBackground();
};


} // gui

#endif  // __PROGRESSBACKGROUND_H__
