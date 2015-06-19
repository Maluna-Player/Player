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
#include "Tools.h"


ProgressBar::ProgressBar(QWidget *parent) : QProgressBar(parent), m_Press(false)
{
    setFixedHeight(PROGRESSBAR_HEIGHT);
}

// ==============================
// ==============================

ProgressBar::~ProgressBar()
{

}

// ==============================
// ==============================

void ProgressBar::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPixmap bar = Tools::loadImage(IMAGES_SUBDIR + "progressBar.png");

    int xBarValue = value() * width() / maximum();
    painter.fillRect(0, 0, xBarValue, bar.height(), QBrush(bar));

    QPixmap marker = Tools::loadImage(IMAGES_SUBDIR + "marker.png");

    painter.drawPixmap(xBarValue - 2, 0, marker);
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
        emit(posChanged(event->x() * 100 / width()));
}

// ==============================
// ==============================

void ProgressBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_Press)
        emit(posChanged(event->x() * 100 / width()));
}

// ==============================
// ==============================

void ProgressBar::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    m_Press = false;
}
