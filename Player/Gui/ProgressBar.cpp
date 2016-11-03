/*************************************
 * @file    ProgressBar.cpp
 * @date    15/08/14
 * @author  Manuel
 *
 * Définitions de la classe ProgressBar.
 *************************************
*/

#include "ProgressBar.h"
#include "Constants.h"
#include "../Util/Tools.h"


namespace gui {


ProgressBar::ProgressBar(QWidget *parent) : QProgressBar(parent), m_Press(false)
{
    setFixedHeight(PROGRESSBAR_HEIGHT);

    m_BarTexture = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "progress_bar.png");
    m_MarkerTexture = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "marker.png");
}

// ==============================
// ==============================

void ProgressBar::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int barWidth = 0;
    if (maximum())
        barWidth = (static_cast<float>(value()) / maximum()) * width();

    painter.fillRect(0, 0, barWidth, m_BarTexture.height(), QBrush(m_BarTexture));
    painter.drawPixmap(barWidth - 2, 0, m_MarkerTexture);
}

// ==============================
// ==============================

void ProgressBar::resizeEvent(QResizeEvent *event)
{
    resize(event->size());
}

// ==============================
// ==============================

void ProgressBar::mousePressEvent(QMouseEvent *event)
{
    int xBarValue = value() * width() / maximum();

    if (event->x() >= xBarValue - 2 && event->x() <= xBarValue + 3)
        m_Press = true;
    else
        emit posChanged(event->x() * 100 / width());
}

// ==============================
// ==============================

void ProgressBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_Press)
        emit posChanged(event->x() * 100 / width());
}

// ==============================
// ==============================

void ProgressBar::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    m_Press = false;
}


} // gui
