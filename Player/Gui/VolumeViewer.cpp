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
#include "Tools.h"


VolumeViewer::VolumeViewer(QWidget *parent) : QLabel(parent)
{
    setFixedSize(Constants::VOLUME_VIEWER_W, Constants::VOLUME_VIEWER_H);
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
    if (volumeState == Constants::MUTE_STATE)
    {
        m_VolumeImage = Tools::loadImage(Constants::IMAGES_SUBDIR + "volumeMute.png");
        m_VolumeValueImage = QPixmap();
    }
    else
    {
        m_VolumeImage = Tools::loadImage(Constants::IMAGES_SUBDIR + "volume.png");
        QPixmap volumeValueImage = Tools::loadImage(Constants::IMAGES_SUBDIR + "volumeValue.png");

        QRect rect(volumeState * Constants::VOLUME_VALUE_W, 0, Constants::VOLUME_VALUE_W, Constants::VOLUME_VALUE_H);
        m_VolumeValueImage = volumeValueImage.copy(rect).scaledToHeight(Constants::VOLUME_VIEWER_H);
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
    painter.drawPixmap(Constants::VOLUME_SPRITE_W + 5, 0, m_VolumeValueImage);
}

// ==============================
// ==============================

void VolumeViewer::mousePressEvent(QMouseEvent *event)
{
    if (event->x() <= Constants::VOLUME_SPRITE_W)
        emit stateChanged();
}
