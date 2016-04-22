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


namespace gui {


class MenuBar : public QMenuBar
{
    private:

        QAction *mp_OpenAction;

        QAction *mp_AddingSongAction;

        QAction *mp_QuitAction;

        QAction *mp_AboutAction;

    public:

        MenuBar(QWidget *parent = nullptr);
        virtual ~MenuBar();

        /**
         * @brief getAddingSongAction
         * @return Retourne le bouton d'ajout de musiques.
         */
        virtual QAction* getAddingSongAction() const;

        /**
         * @brief getOpenAction
         * @return Retourne le bouton d'ouverture de dossier.
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


} // gui

#endif  // __MENUBAR_H__

