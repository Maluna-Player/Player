/*************************************
 * @file    MenuBar.h
 * @date    14/09/15
 * @author  Manuel
 *
 * Déclarations de la classe MenuBar
 * définissant la barre de menu de l'application.
 *************************************
*/

#ifndef __MENUBAR_H__
#define __MENUBAR_H__

#include <QMenuBar>
#include <QAction>

class MenuBar : public QMenuBar
{
    private:

        QAction *mp_OpenAction;

        QAction *mp_QuitAction;

        QAction *mp_AboutAction;

    public:

        MenuBar(QWidget *parent = 0);
        virtual ~MenuBar();

        /**
         * @brief getOpenAction
         * @return Retourne le bouton d'ouverture de fichier.
         */
        virtual QAction* getOpenAction() const;

        /**
         * @brief getQuitAction
         * @return Retourne le bouton de fermeture de l'application.
         */
        virtual QAction* getQuitAction() const;

        /**
         * @brief getAboutAction
         * @return Retourne le bouton "A propos".
         */
        virtual QAction* getAboutAction() const;
};

#endif  // __MENUBAR_H__

