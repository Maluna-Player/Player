/*************************************
 * @file    ProgressBar.h
 * @date    15/08/14
 * @author  Manuel
 *
 * Déclarations de la classe ProgressBar
 * contenant les propriétés de la barre
 * de progression du lecteur.
 *************************************
*/

#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

#include <QProgressBar>
#include <QPainter>
#include <QPaintEvent>

class ProgressBar : public QProgressBar
{
    Q_OBJECT

    private:

        bool m_Press;

    protected:

        virtual void paintEvent(QPaintEvent *event);

        virtual void resizeEvent(QResizeEvent *event);

        virtual void mousePressEvent(QMouseEvent *event);

        virtual void mouseMoveEvent(QMouseEvent *event);

        virtual void mouseReleaseEvent(QMouseEvent *event);

    signals:

        /**
         * @brief Signal émis lors d'un déplacement de la barre de progression
         * @param Nouvelle position en pourcentage
         */
        void posChanged(int);

    public:

        ProgressBar(QWidget *parent = 0);
        virtual ~ProgressBar();
};

#endif  // __PROGRESSBAR_H__
