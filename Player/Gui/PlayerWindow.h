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
#include "ShadowWidget.h"

#include <QTimerEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QVector>
#include <QTimer>
#include <QVBoxLayout>
#include "./Network/PlayerSocket.h"
#include "ConnectionDialog.h"


namespace gui {


class PlayerWindow : public QMainWindow
{
    Q_OBJECT

    private:

        int m_TimerId;

        QString m_PreviewPath;
        QTimer m_PreviewTimer;

        audio::Player m_Player;

        QWidget *mp_TopPart;
        QWidget *mp_BottomPart;

        PlayerLabel *mp_SongTitle;
        PlayerLabel *mp_SongArtist;
        QLabel *mp_SongPicture;
        QPixmap m_DefaultSongPicture;

        Spectrum *mp_Spectrum;
        SongList *mp_SongList;

        PlayerLabel *mp_SongPos;
        PlayerLabel *mp_SongLength;

        ProgressBackground *mp_ProgressBackground;
        NetworkLoadBar *mp_NetworkLoadBar;
        ProgressBar *mp_ProgressBar;

        VolumeViewer *mp_SoundVolume;

        ShadowWidget *mp_ShadowWidget;
        QProgressBar *mp_PreviewBar;

        enum class ButtonId { PLAY, PAUSE, STOP, PREV, NEXT, VOLUME_MORE, VOLUME_LESS, REFRESH };

        QVector<PlayerButton*> mp_Buttons;

        std::unique_ptr<network::PlayerSocket> mp_Socket;

        ConnectionDialog m_ConnectionDialog;

        QPixmap m_ConnectedIcon;
        QPixmap m_DisconnectedIcon;
        QLabel *mp_ConnectionState;


        /**
         * @brief Créé le menu de la fenêtre.
         */
        void createMenuBar();

        /**
         * @brief Créé la barre des options.
         */
        QVBoxLayout* createOptionsBar();

        /**
         * @brief Créé la partie haute de la fenêtre.
         */
        void createTopWindowPart();

        /**
         * @brief Créé la partie basse de la fenêtre.
         */
        void createBottomWindowPart();

        /**
         * @brief Créé le widget de previsualisation.
         */
        void createPreviewWidget();

        /**
         * @brief Récupère le boutton associé à la valeur de l'énumération passée en paramètre.
         * @param id Identifiant du bouton
         * @return Bouton associé à l'identifiant
         */
        PlayerButton* getButton(ButtonId id) const;

        /**
         * @brief Modifie le volume de l'application avec celui passé en paramètre.
         * @param volume Nouveau volume à appliquer
         */
        void setVolume(int volume);

        /**
         * @brief Déplace la position du son selon le décalage passé en paramètre.
         * @param offset Décalage dans le son
         */
        void moveSongPosition(int offset);

    private slots:

        /**
         * @brief Change la visibilité de la liste de musiques (animation).
         * @param visible Nouvelle visibilité de la liste
         */
        void setListVisible(bool visible);

        /**
         * @brief Actualise l'interface graphique (titre, durée...) à partir du son courant.
         */
        void updateCurrentSong();

        /**
         * @brief Met à jour la liste des musiques du répertoire.
         */
        void refreshSongsList();

        /**
         * @brief Change l'état du player et modifie l'affichage.
         * @param state Nouvel état du player
         */
        void setState(PlayerState state);

        /**
         * @brief Applique l'état play.
         */
        void play();

        /**
         * @brief Applique l'état pause.
         */
        void pause();

        /**
         * @brief Applique l'état stop.
         */
        void stop();

        /**
         * @brief Lance la musique précédente si elle existe.
         */
        void previousSong();

        /**
         * @brief Lance la musique suivante si elle existe.
         */
        void nextSong();

        /**
         * @brief Augmente le volume de l'application.
         */
        void increaseVolume();

        /**
         * @brief Diminue le volume de l'application.
         */
        void decreaseVolume();

        /**
         * @brief Modifie la position du son et met à jour la barre de progression.
         * @param value Nouvelle position de la barre
         */
        void setSongPosition(double value);

        /**
         * @brief Applique ou retire l'état mute à l'application.
         */
        void setMute();

        /**
         * @brief Ouvre le dossier des musiques lues par l'application.
         */
        void openSongsDir() const;

        /**
         * @brief Propose à l'utilisateur d'ajouter une nouvelle musique
         *        et l'enregistre si un fichier a été sélectionné.
         */
        void importSong();

        /**
         * @brief Ouvre la fenêtre contenant les informations sur l'application.
         */
        void openInformation();

        /**
         * @brief Ouvre la fenêtre de connexion.
         */
        void openConnection();

        /**
         * @brief Met l'application en écoute de clients.
         */
        void listen();

        /**
         * @brief Se connecte à l'hôte défini.
         * @param host Hôte auquel on essaie de se connecter
         */
        void connectToHost(const QString& host);

        /**
         * @brief Commence la connexion entre les deux hôtes (envoi des musiques).
         */
        void startConnection();

        /**
         * @brief Termine la connexion.
         */
        void closeConnection();

        /**
         * @brief Démarre la preview de la chanson enregistrée.
         */
        void startPreview();

        /**
         * @brief Arrête la preview.
         */
        void stopPreview();

    protected:

        virtual void timerEvent(QTimerEvent *event) override;

        virtual void showEvent(QShowEvent *event) override;

        virtual void hideEvent(QHideEvent *event) override;

        virtual void resizeEvent(QResizeEvent *event) override;

        virtual void paintEvent(QPaintEvent *event) override;

        virtual void dragEnterEvent(QDragEnterEvent *event) override;

        virtual void dragLeaveEvent(QDragLeaveEvent *event) override;

        virtual void dropEvent(QDropEvent *event) override;

    public:

        PlayerWindow(QWidget *parent = nullptr);
        ~PlayerWindow();
};


} // gui

#endif // __PLAYERWINDOW_H__
