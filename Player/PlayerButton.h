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

class PlayerButton : public ClickableLabel
{
    private:

        QString m_Name;


        /**
         * @brief Modifie l'image du bouton par celle passée en paramètre
         * @param fileName Image à charger
         */
        void loadImage(const QString& fileName);

    protected:

        virtual void mousePressEvent(QMouseEvent *event);

        virtual void mouseReleaseEvent(QMouseEvent *event);

    public:

        PlayerButton(const QString& name, QWidget *parent = 0);
        virtual ~PlayerButton();
};

#endif // __PLAYERBUTTON_H__

