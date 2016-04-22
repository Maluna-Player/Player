/*************************************
 * @file    ClickableLabel.cpp
 * @date    03/05/15
 * @author  Manuel
 *
 * Définitions de la classe ClickableLabel.
 *************************************
*/

#include "ClickableLabel.h"


namespace gui {


ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent)
{

}

// ==============================
// ==============================

ClickableLabel::~ClickableLabel()
{

}

// ==============================
// ==============================

void ClickableLabel::mousePressEvent(QMouseEvent * /*event*/)
{

}

// ==============================
// ==============================

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->x() >= 0 && event->x() <= width() && event->y() >= 0 && event->y() <= height())
        emit clicked();
}


} // gui
