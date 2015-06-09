/*************************************
 * @file    VolumeViewer.cpp
 * @date    03/09/14
 * @author  Manuel
 *
 * Définitions de la classe VolumeViewer.
 *************************************
*/

#include "VolumeViewer.h"
#include "Constants.h"
#include "FileLoadingException.h"


VolumeViewer::VolumeViewer(QWidget *parent) : QLabel(parent)
{
    QString volumeImagePath(IMAGES_SUBDIR + "volume.png");
    m_Image.load(volumeImagePath);
    if (m_Image.isNull())
        throw FileLoadingException("VolumeViewer::VolumeViewer", volumeImagePath.toStdString());
}

// ==============================
// ==============================

VolumeViewer::~VolumeViewer()
{

}

// ==============================
// ==============================

void VolumeViewer::setImage(int volumeState)
{
    QRect rect(volumeState * VOLUME_SPRITE_W, 0, VOLUME_SPRITE_W, VOLUME_SPRITE_H);

    setPixmap(m_Image.copy(rect));
}

// ==============================
// ==============================

void VolumeViewer::mousePressEvent(QMouseEvent * /*event*/)
{
    emit(stateChanged());
}
