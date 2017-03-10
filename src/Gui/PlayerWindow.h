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
#include <QMap>
#include <QTimer>
#include <QElapsedTimer>
#include <QToolBar>
#include "./Network/PlayerSocket.h"
#include "ConnectionDialog.h"
#include "OptionBar.h"
#include "ProfileManager.h"


namespace gui {


class PlayerWindow : public QMainWindow
{
    Q_OBJECT

    private:

        enum class PlayerMode { DESKTOP, MINIATURE };

        int m_TimerId;

        PlayerMode m_CurrentMode;

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
        int m_CurrentSpectrumColor;
        QList<SpectrumColor> m_SpectrumColors;

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

        QMap<ButtonId, PlayerButton*> mp_Buttons;

        std::unique_ptr<network::PlayerSocket> mp_Socket;

        ConnectionDialog m_ConnectionDialog;

        QPixmap m_ConnectedIcon;
        QPixmap m_DisconnectedIcon;
        QLabel *mp_ConnectionState;

        QToolBar *mp_Toolbar;
        OptionBar *mp_OptionsBar;

        ProfileManager m_ProfileManager;
        QElapsedTimer m_PlayerTimer;


        /**
         * @brief Créé le menu de la fenêtre.
         */
        void createMenuBar();

        /**
         * @brief Créé la barre des options.
         */
        void createOptionsBar();

        /**
         * @brief Créé la partie haute de la fenêtre.
         */
        void createTopWindowPart();

        /**
         * @brief Créé les barres de progression.
         */
        void createProgressBar();

        /**
         * @brief Créé la partie basse de la fenêtre.
         */
        void createBottomWindowPart();

        /**
         * @brief Créé les options du mode Desktop.
         */
        void createDesktopOptions();

        /**
         * @brief Créé les options du mode miniature.
         */
        void createMiniatureOptions();

        /**
         * @brief Créé le widget de previsualisation.
         */
        void createPreviewWidget();

        /**
         * @brief Créé la fenêtre en mode Desktop.
         */
        void createDesktopWindow();

        /**
         * @brief Change l'agencement de la fenêtre pour passer au mode Desktop.
         */
        void setDesktopLayout();

        /**
         * @brief Change l'agencement de la fenêtre pour passer au mode miniature.
         */
        void setMiniatureLayout();

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

        /**
         * @brief Met à jour le temps de lecture dans le fichier profil.
         */
        void saveListeningTime();

    private slots:

        /**
         * @brief Change la visibilité de la liste de musiques (animation).
         * @param visible Nouvelle visibilité de la liste
         */
        void setListVisible(bool visible);

        /**
         * @brief Change la visibilité des bordures de la fenêtre.
         * @param visible Nouvelle visibilité des bordures
         */
        void setWindowFrameVisible(bool visible);

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
         * @brief Ouvre la fenêtre de personnalisation des couleurs du spectre.
         */
        void openSpectrumColorDialog();

        /**
         * @brief Ouvre la fenêtre de profil.
         */
        void openProfileDialog();

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

        /**
         * @brief Passe l'application en mode Desktop.
         */
        void setDesktopWindow();

        /**
         * @brief Passe l'application en mode miniature.
         */
        void setMiniatureWindow();

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
