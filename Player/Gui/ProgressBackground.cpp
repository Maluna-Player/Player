/*************************************
 * @file    ProgressBackground.cpp
 * @date    16/08/14
 * @author  Manuel
 *
 * Définitions de la classe ProgressBackground.
 *************************************
*/

#include "ProgressBackground.h"
#include "Constants.h"
#include "../Util/Tools.h"


namespace gui {


ProgressBackground::ProgressBackground(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(Constants::PROGRESS_BACKGROUND_HEIGHT);
}

// ==============================
// ==============================

ProgressBackground::~ProgressBackground()
{

}

// ==============================
// ==============================

void ProgressBackground::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPixmap background = util::Tools::loadImage(Constants::IMAGES_SUBDIR + "progressBackground.png");

    painter.fillRect(event->rect(), QBrush(background));
}


} // gui
