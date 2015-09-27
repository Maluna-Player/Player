/*************************************
 * @file    NetworkLoadBar.cpp
 * @date    20/08/15
 * @author  Manuel
 *
 * Définitions de la classe NetworkLoadBar.
 *************************************
*/

#include "NetworkLoadBar.h"
#include "Constants.h"
#include <QPainter>
#include <QResizeEvent>


NetworkLoadBar::NetworkLoadBar(QWidget *parent)
    : QProgressBar(parent), m_StartPos(0)
{
    setFixedHeight(Constants::LOADBAR_HEIGHT);
}

// ==============================
// ==============================

NetworkLoadBar::~NetworkLoadBar()
{

}

// ==============================
// ==============================

void NetworkLoadBar::setStartPos(unsigned int pos)
{
    m_StartPos = pos;
}

// ==============================
// ==============================

void NetworkLoadBar::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int startPosX = m_StartPos * width() / 100;
    int barWidth = 0;
    if (maximum())
        barWidth = (static_cast<float>(value()) / maximum()) * width();

    painter.fillRect(startPosX, 0, barWidth, height(), Qt::gray);
}

// ==============================
// ==============================

void NetworkLoadBar::resizeEvent(QResizeEvent *event)
{
    resize(event->size());
}
