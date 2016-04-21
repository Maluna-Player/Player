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

class VolumeViewer : public QLabel
{
    Q_OBJECT

    private:

        QPixmap m_VolumeImage;
        QPixmap m_VolumeValueImage;

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
        virtual ~VolumeViewer();

        /**
         * @brief Change l'image du viewer de son
         * @param volumeState Volume du player
         */
        virtual void setImage(int volumeState);
};

#endif  // __VOLUMEVIEWER_H__
