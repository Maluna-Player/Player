/*************************************
 * @file    PlayerWindow.h
 * @date    26/04/15
 * @author  Manuel
 *
 * Déclarations de la classe PlayerWindow
 * correspondant à la fenêtre principale.
 *************************************
*/

#ifndef __PLAYERWINDOW_H__
#define __PLAYERWINDOW_H__

#include <QWidget>
#include "Player.h"
#include "Spectrum.h"
#include "PlayerButton.h"

#include <QTimerEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QVector>

class PlayerWindow : public QWidget
{
    Q_OBJECT

    private:

        int m_TimerId;

        Player m_Player;

        QWidget m_TopPart;
        QWidget m_BottomPart;

        Spectrum m_Spectrum;


        enum ButtonId_t { PLAY_BUTTON, PAUSE_BUTTON, STOP_BUTTON, PREV_BUTTON, NEXT_BUTTON };

        QVector<PlayerButton*> mp_Buttons;


        /**
         * @brief Lance le son song du player et actualise l'interface graphique (titre, durée...)
         * @param song Indice de la musique
         */
        virtual void changeSong(int song);

        /**
         * @brief Change l'état du player et modifie l'affichage.
         * @param state Nouvel état du player
         */
        virtual void setState(State_t state);

        /**
         * @brief Met à jour la liste des musiques du répertoire.
         */
        virtual void refreshSongsList();

    private slots:

        virtual void play();

        virtual void pause();

        virtual void stop();

        virtual void previousSong();

        virtual void nextSong();

    protected:

        virtual void timerEvent(QTimerEvent *event);

        virtual void showEvent(QShowEvent *event);

        virtual void hideEvent(QHideEvent *event);

        virtual void resizeEvent(QResizeEvent *event);

    public:

        PlayerWindow(QWidget *parent = 0);
        virtual ~PlayerWindow();
};

#endif // __PLAYERWINDOW_H__
