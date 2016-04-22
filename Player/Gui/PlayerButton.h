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
#include <QString>


namespace gui {


class PlayerButton : public ClickableLabel
{
    private:

        QString m_Name;


        /**
         * @brief Modifie l'image du bouton par celle passée en paramètre
         * @param fileName Image à charger
         */
        virtual void loadImage(const QString& fileName);

    protected:

        virtual void mousePressEvent(QMouseEvent *event) override;

        virtual void mouseReleaseEvent(QMouseEvent *event) override;

    public:

        PlayerButton(const QString& name, QWidget *parent = nullptr);
        virtual ~PlayerButton();
};


} // gui

#endif // __PLAYERBUTTON_H__

