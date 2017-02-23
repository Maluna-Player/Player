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
#include <QMap>
#include <QIcon>
#include <functional>


namespace gui {


class PlayerToggleButton : public QPushButton
{
    private:

        QMap<bool, QIcon> m_Icons;

        QMap<bool, QString> m_Tooltips;

        std::function<void(bool)> m_CommonAction;
        QMap<bool, std::function<void(bool)>> m_Actions;

    private slots:

        /**
         * @brief Change l'état du bouton (icône, tooltip, action).
         * @param newState Nouvel état
         */
        void changeState(bool newState);

    public:

        PlayerToggleButton(const QString& filename = "", bool enabled = false, QWidget *parent = nullptr);
        virtual ~PlayerToggleButton() = default;

        /**
         * @brief Change l'icône globale du bouton.
         * @param icon Nouvelle icône
         */
        void setIcon(const QIcon& icon);

        /**
         * @brief Change l'icône du bouton pour l'état passé en paramètre.
         * @param checked Etat auquel associer l'icône
         * @param icon Icône à associer
         */
        void setIcon(bool checked, const QIcon& icon);

        /**
         * @brief Change l'infobulle globale du bouton.
         * @param tooltip Nouvelle infobulle
         */
        void setToolTip(const QString& tooltip);

        /**
         * @brief Change l'infobulle du bouton pour l'état passé en paramètre.
         * @param checked Etat auquel associer l'infobulle
         * @param tooltip Infobulle à associer
         */
        void setToolTip(bool checked, const QString& tooltip);

        /**
         * @brief Change l'action globale associée au bouton.
         * @param f Fonction à exécuter
         */
        void setAction(const std::function<void(bool)>& f);

        /**
         * @brief Change l'action du bouton pour l'état passé en paramètre.
         * @param checked Etat auquel associer l'action
         * @param f Fonction à associer
         */
        void setAction(bool checked, const std::function<void(bool)>& f);
};


} // gui

#endif  // __PLAYERTOGGLEBUTTON_H__
