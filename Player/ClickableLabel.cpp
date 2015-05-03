/*************************************
 * @file    ClickableLabel.cpp
 * @date    03/05/15
 * @author  Manuel
 *
 * Définitions de la classe ClickableLabel.
 *************************************
*/

#include "ClickableLabel.h"


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

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{

}

// ==============================
// ==============================

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked();
}
