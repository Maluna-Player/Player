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
#include <QPixmap>


namespace gui {


PlayerButton::PlayerButton(const QString& name, QWidget *parent)
    : ClickableLabel(parent), m_Name(name)
{
    loadImage(m_Name + ".png");
}

// ==============================
// ==============================

PlayerButton::~PlayerButton()
{

}

// ==============================
// ==============================

void PlayerButton::loadImage(const QString& fileName)
{
    QPixmap image = util::Tools::loadImage(BUTTONS_SUBDIR + fileName);

    setPixmap(image.scaled(70, 70, Qt::KeepAspectRatio));
}

// ==============================
// ==============================

void PlayerButton::mousePressEvent(QMouseEvent *event)
{
    loadImage(m_Name + "p.png");

    ClickableLabel::mousePressEvent(event);
}

// ==============================
// ==============================

void PlayerButton::mouseReleaseEvent(QMouseEvent *event)
{
    loadImage(m_Name + ".png");

    ClickableLabel::mouseReleaseEvent(event);
}


} // gui
