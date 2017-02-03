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
    setFixedHeight(PROGRESS_BACKGROUND_HEIGHT);

    m_BackgroundTexture = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "progress_background.png");
}

// ==============================
// ==============================

void ProgressBackground::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(event->rect(), QBrush(m_BackgroundTexture));
}


} // gui
