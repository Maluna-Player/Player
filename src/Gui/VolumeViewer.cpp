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


VolumeViewer::VolumeViewer(QWidget *parent) : QLabel(parent), m_Pos(0.0)
{
    setFixedSize(VOLUME_VIEWER_W, VOLUME_VIEWER_H);

    m_VolumeTexture = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "volume.png");
    m_MuteTexture = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "volume_mute.png");
}

// ==============================
// ==============================

void VolumeViewer::setState(int volumeState)
{
    if (volumeState == MUTE_STATE)
    {
        m_VolumeImage = m_MuteTexture;
        m_Pos = -1.0;
    }
    else
    {
        m_VolumeImage = m_VolumeTexture;
        m_Pos = volumeState * (1.0 / (NB_VOLUME_STATES - 1));
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


    if (m_Pos >= 0.0)
    {
        QPen pen(QColor(76, 112, 166));
        pen.setWidth(2);
        painter.setPen(pen);

        QRect borderRect { VOLUME_VALUE_X, 1, VOLUME_VALUE_W, VOLUME_VALUE_H };

        if (m_Pos > 0)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);

            QRectF valueRect { VOLUME_VALUE_X, VOLUME_VALUE_H - VOLUME_VALUE_H * m_Pos,
                                VOLUME_VALUE_W, VOLUME_VALUE_H * m_Pos };

            if (m_Pos < 0.2)
            {
                path.addRoundedRect(QRectF(valueRect.left() + 1, valueRect.top(), valueRect.width() - 2, valueRect.height()), 10, 10);
                path.addRect(QRectF(valueRect.topLeft(), QSize(valueRect.width(), 3)));
            }
            else
            {
                path.addRoundedRect(valueRect, 10, 10);

                if (m_Pos < 1.0)
                    path.addRect(QRectF(valueRect.topLeft(), QSize(valueRect.width(), 5)));
            }

            QLinearGradient gradient(borderRect.topLeft(), borderRect.bottomRight());
            gradient.setColorAt(0.0, QColor(170, 0, 0));
            gradient.setColorAt(1.0, QColor(255, 214, 214));

            painter.fillPath(path.simplified(), gradient);
        }

        painter.drawRoundedRect(borderRect, 10, 10);
    }
}

// ==============================
// ==============================

void VolumeViewer::mousePressEvent(QMouseEvent *event)
{
    if (event->x() <= VOLUME_SPRITE_W)
        emit stateChanged();
}


} // gui
