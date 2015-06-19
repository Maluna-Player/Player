/*************************************
 * @file    PlayerLabel.cpp
 * @date    08/06/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerLabel.
 *************************************
*/

#include "PlayerLabel.h"


PlayerLabel::PlayerLabel(QWidget *parent)
    : QLabel(parent)
{

}

// ==============================
// ==============================

PlayerLabel::PlayerLabel(const QColor& color, int size, QWidget *parent)
    : QLabel(parent)
{
    setColor(color);

    QFont font;
    font.setPointSize(size);
    setFont(font);
}

// ==============================
// ==============================

PlayerLabel::PlayerLabel(const QColor& color, const QFont& font, QWidget *parent)
    : QLabel(parent)
{
    setColor(color);
    setFont(font);
}

// ==============================
// ==============================

PlayerLabel::~PlayerLabel()
{

}

// ==============================
// ==============================

void PlayerLabel::setColor(const QColor& color)
{
    QPalette pal(palette());
    pal.setColor(QPalette::WindowText, color);
    setPalette(pal);
}

// ==============================
// ==============================

void PlayerLabel::setBold(bool enable)
{
    QFont f = font();
    f.setBold(enable);
    setFont(f);
}

// ==============================
// ==============================

void PlayerLabel::setItalic(bool enable)
{
    QFont f = font();
    f.setItalic(enable);
    setFont(f);
}
