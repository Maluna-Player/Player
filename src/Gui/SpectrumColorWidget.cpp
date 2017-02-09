/*************************************
 * @file    SpectrumColorWidget.cpp
 * @date    03/02/17
 * @author  Manuel
 *
 * Définitions de la classe SpectrumColorWidget.
 *************************************
*/

#include "SpectrumColorWidget.h"
#include <QPainter>


namespace gui {


SpectrumColorWidget::SpectrumColorWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(WIDTH, HEIGHT);
}

// ==============================
// ==============================

SpectrumColorWidget::SpectrumColorWidget(const SpectrumColor& color, QWidget *parent)
    : QWidget(parent), m_Color(color)
{
    setFixedSize(WIDTH, HEIGHT);
}

// ==============================
// ==============================

SpectrumColor SpectrumColorWidget::getColor() const
{
    return m_Color;
}

// ==============================
// ==============================

void SpectrumColorWidget::setColor(const SpectrumColor& color)
{
    m_Color = color;
}

// ==============================
// ==============================

void SpectrumColorWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient gradient(0, 0, WIDTH, HEIGHT);
    gradient.setColorAt(0.0, m_Color.topColor);
    gradient.setColorAt(1.0, m_Color.bottomColor);

    painter.setPen(Qt::black);
    painter.setBrush(gradient);
    painter.drawRoundedRect(0, 0, WIDTH, HEIGHT, 8, 8);
}


} // gui
