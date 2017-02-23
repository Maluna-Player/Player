/*************************************
 * @file    VolumeViewer.h
 * @date    03/09/14
 * @author  Manuel
 *
 * Déclarations de la classe VolumeViewer
 * correspondant à l'afficheur du volume.
 *************************************
*/

#ifndef __VOLUMEVIEWER_H__
#define __VOLUMEVIEWER_H__

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>


namespace gui {


class VolumeViewer : public QLabel
{
    Q_OBJECT

    private:

        QPixmap m_VolumeTexture;
        QPixmap m_MuteTexture;

        QPixmap m_VolumeImage;
        double m_Pos;

    protected:

        virtual void paintEvent(QPaintEvent *event) override;

        virtual void mousePressEvent(QMouseEvent *event) override;

    signals:

        /**
         * @brief Signal émis lors du clic sur le volume
         */
        void stateChanged();

    public:

        VolumeViewer(QWidget *parent = nullptr);
        virtual ~VolumeViewer() = default;

        /**
         * @brief Change l'image du viewer de son
         * @param volumeState Volume du player
         */
        void setState(int volumeState);
};


} // gui

#endif  // __VOLUMEVIEWER_H__
