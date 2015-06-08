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

class VolumeViewer : public QLabel
{
    private:

        QPixmap m_Image;

    public:

        VolumeViewer(QWidget *parent = 0);
        virtual ~VolumeViewer();

        /**
         * @brief Change l'image du viewer de son
         * @param volumeState Volume du player
         */
        virtual void setImage(int volumeState);
};

#endif  // __VOLUMEVIEWER_H__
