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
#include "../Util/Tools.h"


namespace gui {


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
        m_VolumeImage = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "volumeMute.png");
        m_VolumeValueImage = QPixmap();
    }
    else
    {
        m_VolumeImage = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "volume.png");
        QPixmap volumeValueImage = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "volumeValue.png");

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
        emit stateChanged();
}


} // gui
