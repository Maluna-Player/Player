/*************************************
 * @file    ShadowWidget.cpp
 * @date    02/09/16
 * @author  Manuel
 *
 * Définitions de la classe ShadowWidget.
 *************************************
*/

#include "ShadowWidget.h"
#include <QPainter>


namespace gui {


ShadowWidget::ShadowWidget(int r, int g, int b, int alpha, QWidget *parent)
    : QWidget(parent), m_Color(r, g, b, alpha)
{

}

// ==============================
// ==============================

ShadowWidget::ShadowWidget(int alpha, QWidget *parent)
    : ShadowWidget(0, 0, 0, alpha, parent)
{

}

// ==============================
// ==============================

ShadowWidget::ShadowWidget(const QColor& color, QWidget *parent)
    : QWidget(parent), m_Color(color)
{

}

// ==============================
// ==============================

int ShadowWidget::getOpacity() const
{
    return m_Color.alpha();
}

// ==============================
// ==============================

void ShadowWidget::setOpacity(int alpha)
{
    m_Color.setAlpha(alpha);
    update();
}

// ==============================
// ==============================

void ShadowWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(event->rect(), m_Color);
}


} // gui
