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
#include "FileLoadingException.h"


ProgressBackground::ProgressBackground(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(PROGRESS_BACKGROUND_HEIGHT);
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

    QString imagePath(IMAGES_SUBDIR + "progressBackground.png");
    QPixmap background(imagePath);
    if (background.isNull())
        throw FileLoadingException("ProgressBackground::paintEvent", imagePath.toStdString());

    painter.fillRect(event->rect(), QBrush(background));
}
