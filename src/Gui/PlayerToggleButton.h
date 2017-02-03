/*************************************
 * @file    PlayerToggleButton.h
 * @date    27/10/16
 * @author  Manuel
 *
 * Déclarations de la classe PlayerToggleButton
 * définissant un toggle bouton.
 *************************************
*/

#ifndef __PLAYERTOGGLEBUTTON_H__
#define __PLAYERTOGGLEBUTTON_H__

#include <QPushButton>


namespace gui {


class PlayerToggleButton : public QPushButton
{
    public:

        PlayerToggleButton(const QString& filename, bool enabled = false, QWidget *parent = nullptr);
        virtual ~PlayerToggleButton() = default;
};


} // gui

#endif  // __PLAYERTOGGLEBUTTON_H__
