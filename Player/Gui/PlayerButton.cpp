/*************************************
 * @file    PlayerButton.cpp
 * @date    03/05/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerButton.
 *************************************
*/

#include "PlayerButton.h"
#include "Constants.h"
#include "../Util/Tools.h"


namespace gui {


PlayerButton::PlayerButton(const QString& name, QWidget *parent)
    : ClickableLabel(parent), m_Name(name)
{
    m_ButtonTexture = util::Tools::loadImage(BUTTONS_SUBDIR + m_Name + ".png");
    m_PressedButtonTexture = util::Tools::loadImage(BUTTONS_SUBDIR + m_Name + "p.png");

    loadImage(m_ButtonTexture);
}

// ==============================
// ==============================

void PlayerButton::loadImage(const QPixmap& image)
{
    setPixmap(image.scaled(70, 70, Qt::KeepAspectRatio));
}

// ==============================
// ==============================

void PlayerButton::mousePressEvent(QMouseEvent *event)
{
    loadImage(m_PressedButtonTexture);

    ClickableLabel::mousePressEvent(event);
}

// ==============================
// ==============================

void PlayerButton::mouseReleaseEvent(QMouseEvent *event)
{
    loadImage(m_ButtonTexture);

    ClickableLabel::mouseReleaseEvent(event);
}


} // gui
