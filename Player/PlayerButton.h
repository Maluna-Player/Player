/*************************************
 * @file    PlayerButton.h
 * @date    03/05/15
 * @author  Manuel
 *
 * Déclarations de la classe PlayerButton
 * correspondant à un bouton de l'application.
 *************************************
*/

#ifndef __PLAYERBUTTON_H__
#define __PLAYERBUTTON_H__

#include "ClickableLabel.h"

class PlayerButton : public ClickableLabel
{
    public:

        PlayerButton(const QString& fileName, QWidget *parent = 0);
        virtual ~PlayerButton();
};

#endif // __PLAYERBUTTON_H__

