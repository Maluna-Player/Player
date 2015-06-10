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
    setFixedSize(VOLUME_VIEWER_W, VOLUME_VIEWER_H);
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
    if (volumeState == MUTE_STATE)
    {
        QString volumeImagePath(IMAGES_SUBDIR + "volumeMute.png");
        m_VolumeImage.load(volumeImagePath);
        if (m_VolumeImage.isNull())
            throw FileLoadingException("VolumeViewer::setImage", volumeImagePath.toStdString());

        m_VolumeValueImage = QPixmap();
    }
    else
    {
        QString volumeImagePath(IMAGES_SUBDIR + "volume.png");
        m_VolumeImage.load(volumeImagePath);
        if (m_VolumeImage.isNull())
            throw FileLoadingException("VolumeViewer::setImage", volumeImagePath.toStdString());

        QString volumeValueImagePath(IMAGES_SUBDIR + "volumeValue.png");
        QPixmap volumeValueImage(volumeValueImagePath);
        if (volumeValueImage.isNull())
            throw FileLoadingException("VolumeViewer::setImage", volumeValueImagePath.toStdString());

        QRect rect(volumeState * VOLUME_VALUE_W, 0, VOLUME_VALUE_W, VOLUME_VALUE_H);

        m_VolumeValueImage = volumeValueImage.copy(rect).scaledToHeight(VOLUME_VIEWER_H);
    }

    update();
}

// ==============================
// ==============================

void VolumeViewer::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawPixmap(0, 0, m_VolumeImage);
    painter.drawPixmap(VOLUME_SPRITE_W + 5, 0, m_VolumeValueImage);
}

// ==============================
// ==============================

void VolumeViewer::mousePressEvent(QMouseEvent *event)
{
    if (event->x() <= VOLUME_SPRITE_W)
        emit(stateChanged());
}
