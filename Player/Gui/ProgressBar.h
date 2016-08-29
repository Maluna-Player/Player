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


namespace gui {


class ProgressBar : public QProgressBar
{
    Q_OBJECT

    private:

        QPixmap m_BarTexture;

        QPixmap m_MarkerTexture;

        bool m_Press;

    protected:

        virtual void paintEvent(QPaintEvent *event) override;

        virtual void resizeEvent(QResizeEvent *event) override;

        virtual void mousePressEvent(QMouseEvent *event) override;

        virtual void mouseMoveEvent(QMouseEvent *event) override;

        virtual void mouseReleaseEvent(QMouseEvent *event) override;

    signals:

        /**
         * @brief Signal émis lors d'un déplacement de la barre de progression
         * @param Nouvelle position en pourcentage
         */
        void posChanged(int);

    public:

        ProgressBar(QWidget *parent = nullptr);
        virtual ~ProgressBar();
};


} // gui

#endif  // __PROGRESSBAR_H__
