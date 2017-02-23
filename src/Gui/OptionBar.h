/*************************************
 * @file    OptionBar.h
 * @date    17/02/17
 * @author  Manuel
 *
 * Déclarations de la classe OptionBar
 * définissant la barre d'options de l'application.
 *************************************
*/

#ifndef __OPTIONBAR_H__
#define __OPTIONBAR_H__

#include <QWidget>
#include <QVBoxLayout>
#include "PlayerToggleButton.h"


namespace gui {


class OptionBar : public QWidget
{
    private:

        QVBoxLayout *mp_Layout;

    public:

        OptionBar(QWidget *parent = nullptr);
        virtual ~OptionBar() = default;

        /**
         * @brief Ajoute le bouton passé en paramètre à l'index indiqué de la barre.
         * @param index Index auquel ajouter le bouton
         * @param button Bouton à ajouter
         */
        void addButton(int index, PlayerToggleButton *button);

        /**
         * @brief Supprime le bouton à l'index passé en paramètre.
         * @param index Index du bouton à supprimer
         */
        void removeButton(int index);
};


} // gui

#endif  // __OPTIONBAR_H__
