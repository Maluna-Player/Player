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
    : ClickableLabel(parent), m_Name(name), m_Pressed(false), m_Released(true)
{
    m_ButtonTexture = util::Tools::loadImage(BUTTONS_SUBDIR + m_Name + ".png");
    m_PressedButtonTexture = util::Tools::loadImage(BUTTONS_SUBDIR + m_Name + "p.png");

    loadImage(m_ButtonTexture);

    m_PressTimer.setSingleShot(true);
    m_PressTimer.setInterval(BUTTON_DELAY);
    connect(&m_PressTimer, &QTimer::timeout, this, &PlayerButton::press);
}

// ==============================
// ==============================

void PlayerButton::loadImage(const QPixmap& image)
{
    setPixmap(image.scaled(70, 70, Qt::KeepAspectRatio));
}

// ==============================
// ==============================

bool PlayerButton::isPressed() const
{
    return m_Pressed;
}

// ==============================
// ==============================

void PlayerButton::press()
{
    m_Pressed = true;
    m_Released = false;
}

// ==============================
// ==============================

void PlayerButton::release()
{
    m_Pressed = false;
}

// ==============================
// ==============================

void PlayerButton::mousePressEvent(QMouseEvent *event)
{
    loadImage(m_PressedButtonTexture);

    m_PressTimer.start();

    ClickableLabel::mousePressEvent(event);
}

// ==============================
// ==============================

void PlayerButton::mouseReleaseEvent(QMouseEvent *event)
{
    loadImage(m_ButtonTexture);

    if (m_Released)
        ClickableLabel::mouseReleaseEvent(event);

    m_Released = true;
    emit released();

    m_PressTimer.stop();
    release();
}

// ==============================
// ==============================

void PlayerButton::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressed())
    {
        if (event->x() < 0 || event->x() > width() || event->y() < 0 || event->y() > height())
            mouseLeaveEvent();
    }
}

// ==============================
// ==============================

void PlayerButton::mouseLeaveEvent()
{
    m_PressTimer.stop();
    release();
}


} // gui
