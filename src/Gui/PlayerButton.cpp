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
    : ClickableLabel(parent), m_Name(name), m_Pressable(false), m_Pressed(false), m_ActivePressure(false)
{
    m_ButtonTexture = util::Tools::loadImage(BUTTONS_SUBDIR + m_Name + ".png");
    m_PressedButtonTexture = util::Tools::loadImage(BUTTONS_SUBDIR + m_Name + "p.png");
    m_HoverButtonTexture = util::Tools::loadImage(BUTTONS_SUBDIR + m_Name + "h.png");

    loadImage(m_ButtonTexture);

    m_PressTimer.setSingleShot(true);
    m_PressTimer.setInterval(BUTTON_DELAY);
    connect(&m_PressTimer, &QTimer::timeout, this, &PlayerButton::activatePressure);
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
    return m_ActivePressure;
}

// ==============================
// ==============================

void PlayerButton::setPressable(bool value)
{
    m_Pressable = value;
}

// ==============================
// ==============================

void PlayerButton::activatePressure()
{
    m_Pressed = true;
    m_ActivePressure = true;
}

// ==============================
// ==============================

void PlayerButton::unpress()
{
    m_ActivePressure = false;
}

// ==============================
// ==============================

void PlayerButton::mousePressEvent(QMouseEvent *event)
{
    loadImage(m_PressedButtonTexture);

    if (m_Pressable)
        m_PressTimer.start();

    ClickableLabel::mousePressEvent(event);
}

// ==============================
// ==============================

void PlayerButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->x() < 0 || event->x() > width() || event->y() < 0 || event->y() > height())
        loadImage(m_ButtonTexture);
    else
        loadImage(m_HoverButtonTexture);

    if (!m_Pressable || !m_Pressed)
        ClickableLabel::mouseReleaseEvent(event);

    m_PressTimer.stop();
    unpress();
    m_Pressed = false;

    emit released();
}

// ==============================
// ==============================

void PlayerButton::mouseMoveEvent(QMouseEvent *event)
{
    if (event->x() < 0 || event->x() > width() || event->y() < 0 || event->y() > height())
        mouseLeaveEvent();
}

// ==============================
// ==============================

void PlayerButton::enterEvent(QEvent * /*event*/)
{
    loadImage(m_HoverButtonTexture);
}

// ==============================
// ==============================

void PlayerButton::leaveEvent(QEvent * /*event*/)
{
    loadImage(m_ButtonTexture);
}

// ==============================
// ==============================

void PlayerButton::mouseLeaveEvent()
{
    m_PressTimer.stop();
    unpress();
}


} // gui
