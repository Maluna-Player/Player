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

#include <QMainWindow>
#include "../Audio/Player.h"
#include "Spectrum.h"
#include "PlayerLabel.h"
#include "PlayerButton.h"
#include "ProgressBackground.h"
#include "NetworkLoadBar.h"
#include "ProgressBar.h"
#include "VolumeViewer.h"
#include "SongList.h"

#include <QTimerEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QVector>
#include "./Network/PlayerSocket.h"
#include "ConnectionBox.h"

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

    private:

        int m_TimerId;

        Player m_Player;

        QWidget *mp_TopPart;
        QWidget *mp_BottomPart;

        PlayerLabel *mp_SongTitle;
        PlayerLabel *mp_SongArtist;
        QLabel *mp_SongPicture;

        Spectrum *mp_Spectrum;
        SongList *mp_SongList;

        PlayerLabel *mp_SongPos;
        PlayerLabel *mp_SongLength;

        ProgressBackground *mp_ProgressBackground;
        NetworkLoadBar *mp_NetworkLoadBar;
        ProgressBar *mp_ProgressBar;

        VolumeViewer *mp_SoundVolume;

        enum ButtonId_t { PLAY_BUTTON, PAUSE_BUTTON, STOP_BUTTON, PREV_BUTTON, NEXT_BUTTON,
                          VOLUME_MORE_BUTTON, VOLUME_LESS_BUTTON };

        QVector<PlayerButton*> mp_Buttons;

        PlayerSocket *mp_Socket;

        ConnectionBox *mp_ConnectionBox;


        /**
         * @brief Change l'état du player et modifie l'affichage.
         * @param state Nouvel état du player
         */
        virtual void setState(State_t state);

        /**
         * @brief Met à jour la liste des musiques du répertoire.
         */
        virtual void refreshSongsList();

        /**
         * @brief Modifie le volume de l'application avec celui passé en paramètre.
         * @param volume Nouveau volume à appliquer
         */
        virtual void setVolume(int volume);

    private slots:

        /**
         * @brief Lance le son song du player et actualise l'interface graphique (titre, durée...)
         * @param song Indice de la musique
         */
        virtual void changeSong(int song);

        /**
         * @brief Applique l'état play.
         */
        virtual void play();

        /**
         * @brief Applique l'état pause.
         */
        virtual void pause();

        /**
         * @brief Applique l'état stop.
         */
        virtual void stop();

        /**
         * @brief Lance la musique précédente si elle existe.
         */
        virtual void previousSong();

        /**
         * @brief Lance la musique suivante si elle existe.
         */
        virtual void nextSong();

        /**
         * @brief Augmente le volume de l'application.
         */
        virtual void increaseVolume();

        /**
         * @brief Diminue le volume de l'application.
         */
        virtual void decreaseVolume();

        /**
         * @brief Modifie la position du son et met à jour la barre de progression.
         * @param value Nouvelle position de la barre
         */
        virtual void setSongPosition(int value);

        /**
         * @brief Applique ou retire l'état mute à l'application.
         */
        virtual void setMute();

        /**
         * @brief Ouvre le dossier des musiques lues par l'application.
         */
        virtual void openSongsDir() const;

        /**
         * @brief Propose à l'utilisateur d'ajouter une nouvelle musique
         *        et l'enregistre si un fichier a été sélectionné.
         */
        virtual void importSong();

        /**
         * @brief Ouvre la fenêtre contenant les informations sur l'application.
         */
        virtual void openInformation();

        /**
         * @brief Met l'application en écoute de clients.
         */
        virtual void listen();

        /**
         * @brief Se connecte à l'hôte défini.
         * @param host Hôte auquel on essaie de se connecter
         */
        virtual void connectToHost(const QString& host);

        /**
         * @brief Commence la connexion entre les deux hôtes (envoi des musiques).
         */
        virtual void startConnection();

        /**
         * @brief Termine la connexion.
         */
        virtual void closeConnection();

    protected:

        virtual void timerEvent(QTimerEvent *event);

        virtual void showEvent(QShowEvent *event);

        virtual void hideEvent(QHideEvent *event);

        virtual void resizeEvent(QResizeEvent *event);

        virtual void paintEvent(QPaintEvent *event);

    public:

        PlayerWindow(QWidget *parent = 0);
        virtual ~PlayerWindow();
};

#endif // __PLAYERWINDOW_H__
