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
#include <QPixmap>
#include <QTimer>


namespace gui {


class PlayerButton : public ClickableLabel
{
    Q_OBJECT

    private:

        QString m_Name;

        QPixmap m_ButtonTexture;

        QPixmap m_PressedButtonTexture;

        QTimer m_PressTimer;

        bool m_Pressed;

        bool m_Released;


        /**
         * @brief Modifie l'image du bouton par celle passée en paramètre
         * @param image Image à charger
         */
        void loadImage(const QPixmap& image);

    protected:

        virtual void mousePressEvent(QMouseEvent *event) override;

        virtual void mouseReleaseEvent(QMouseEvent *event) override;

        virtual void mouseMoveEvent(QMouseEvent *event) override;

        virtual void mouseLeaveEvent();

    private slots:

        void press();

    signals:

        void released();

    public:

        PlayerButton(const QString& name, QWidget *parent = nullptr);
        virtual ~PlayerButton() = default;

        /**
         * @brief Détermine si le bouton est pressé ou non.
         * @return true si le bouton est pressé
         */
        bool isPressed() const;

        /**
         * @brief Relâche le bouton.
         */
        void release();
};


} // gui

#endif // __PLAYERBUTTON_H__

