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
#include "FileLoadingException.h"
#include <QPixmap>


PlayerButton::PlayerButton(const QString& fileName, QWidget *parent)
    : ClickableLabel(parent)
{
    QPixmap image(fileName);
    if (image.isNull())
        throw FileLoadingException("PlayerButton::PlayerButton", fileName.toStdString());

    setPixmap(image.scaled(BUTTON_SIZE, BUTTON_SIZE, Qt::KeepAspectRatio));
}

// ==============================
// ==============================

PlayerButton::~PlayerButton()
{

}

// ==============================
// ==============================
