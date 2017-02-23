/*************************************
 * @file    OptionBar.cpp
 * @date    17/02/17
 * @author  Manuel
 *
 * Définitions de la classe OptionBar.
 *************************************
*/

#include "OptionBar.h"


namespace gui {


OptionBar::OptionBar(QWidget *parent)
    : QWidget(parent)
{
    mp_Layout = new QVBoxLayout;
    mp_Layout->setAlignment(Qt::AlignTop);

    mp_Layout->addStretch(1);

    setLayout(mp_Layout);
}

// ==============================
// ==============================

void OptionBar::addButton(int index, PlayerToggleButton *button)
{
    mp_Layout->insertWidget(index, button);
}

// ==============================
// ==============================

void OptionBar::removeButton(int index)
{
    if (index < mp_Layout->count() - 1)
    {
        QLayoutItem *item = mp_Layout->takeAt(index);

        if (item)
        {
            delete item->widget();
            delete item;
        }
    }
}


} // gui
