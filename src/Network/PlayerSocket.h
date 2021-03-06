/*************************************
 * @file    PlayerSocket.h
 * @date    16/06/15
 * @author  Manuel
 *
 * Déclarations de la classe PlayerSocket
 * définissant une socket de communication
 * avec un deuxième client.
 *************************************
*/

#ifndef __PLAYERSOCKET_H__
#define __PLAYERSOCKET_H__

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include "../Gui/SongListItem.h"
#include "PlayerMessageBox.h"
#include "Commands/Command.h"
#include "Commands/CommandRequest.h"
#include "Commands/CommandReply.h"
#include "../Audio/FmodManager.h"
#include "../Audio/Player.h"


namespace network {


class PlayerSocket : public QObject
{
    Q_OBJECT

    private:

        audio::Player *mp_Player;

        bool m_Connected;

        QTcpServer *mp_Server;
        QTcpSocket *mp_Socket;

        int m_NbSentListItems;
        int m_NbReceivedSongs;

        std::unique_ptr<PlayerMessageBox> mp_MessageBox;

        QThread *mp_SocketThread;

        QVector<std::shared_ptr<commands::CommandRequest>> mp_ReceivedRequests;
        QVector<std::shared_ptr<commands::CommandReply>> mp_ReceivedReplies;

        audio::SoundSettings m_CallbackSettings;

        quint32 m_SongDataReceived;
        quint32 m_TotalCurrentSongData;


        /**
         * @brief Cherche l'item correspondant au numéro passé en paramètre dans l'arborescence parent.
         * @param num Numéro de l'item à chercher
         * @param parent Racine de l'arborescence dans laquelle chercher
         * @return Item correspondant au numéro indiqué
         */
        gui::SongListItem* getItem(int num, gui::SongTreeRoot *parent) const;

        /**
         * @brief Envoie la liste des éléments récursivement à partir de item.
         * @param item Element racine à partir duquel envoyer la liste
         */
        void sendSongs(gui::SongTreeRoot *item);

        /**
         * @brief Lit les données des musiques distantes reçues sur le socket.
         * @return Liste des musiques reçues
         */
        gui::SongTreeRoot* readRemoteSongList();

        /**
         * @brief Construit l'objet Command à partir du message passé en paramètre.
         * @param message Message contenant la commande
         * @return Commande construite
         */
        std::shared_ptr<commands::Command> buildCommand(QByteArray message) const;

        /**
         * @brief Récupère la prochaine requête parmi la liste des messages traités ou récupérés.
         * @return Prochaine requête du client
         */
        std::shared_ptr<commands::CommandRequest> getCommandRequest();

        /**
         * @brief Récupère la prochaine réponse parmi la liste des messages traités ou récupérés.
         * @return Prochaine réponse du client
         */
        std::shared_ptr<commands::CommandReply> getCommandReply();

    private slots:

        /**
         * @brief Se connecte avec le client demandé.
         */
        void clientConnexion();

        /**
         * @brief Envoie la liste des musiques.
         */
        void startConnection();

        /**
         * @brief Affiche l'erreur de connexion.
         */
        void error();

    signals:

        /**
         * @brief Signal émis lorsque la connexion entre les deux utilisateurs est établie.
         */
        void connected();

        /**
         * @brief Signal émis lors de la fin de la connexion avec un autre client.
         */
        void disconnected();

        /**
         * @brief Signal émis lorsqu'une commande est reçue.
         * @param Commande reçue
         */
        void commandReceived(std::shared_ptr<commands::CommandRequest>);

    public:

        PlayerSocket(audio::Player *player);
        virtual ~PlayerSocket();

        /**
         * @brief getCallbackSettings
         * @return Options fmod pour la lecture réseau
         */
        audio::SoundSettings& getCallbackSettings();

        /**
         * @brief getSongDataReceived
         * @return Taille des données du son distant reçues
         */
        quint32 getSongDataReceived() const;

        /**
         * @brief getTotalCurrentSongData
         * @return Taille totale des données du son distant
         */
        quint32 getTotalCurrentSongData() const;

        /**
         * @brief isConnected
         * @return true si la connexion entre les deux clients est établie et les listes échangées.
         */
        bool isConnected() const;

        /**
         * @brief Met le socket serveur en écoute de clients.
         * @param address Adresse sur laquelle écouter les connexions entrantes
         */
        void listen(QHostAddress address);

        /**
         * @brief Se connecte à l'hôte passé en paramètre par le port défini par l'application.
         * @param address Adresse de l'hôte
         */
        void connectToHost(const QString& address);

        /**
         * @brief Envoie au client la liste des musiques enregistrées et reçoit ses musiques.
         * @param songs Arborescence des musiques à envoyer
         * @return Arborescence des musiques de l'autre client
         */
        gui::SongTreeRoot* exchangeSongList(gui::SongTreeRoot *songs);

        /**
         * @brief Vérifie s'il existe une prochaine requête client et signale sa réception.
         */
        void processCommands();


        /** Méthodes de callback appelées par les fonctions pour le stream de musique distantes **/

        FMOD_RESULT openRemoteFile(const char *fileName, unsigned int *filesize, void **handle);
        FMOD_RESULT closeRemoteFile(void *handle);
        FMOD_RESULT readRemoteFile(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread);
        FMOD_RESULT seekRemoteFile(void *handle, unsigned int pos);

    public slots:

        /**
         * @brief Ferme le socket et signale à l'application la fin de la communication.
         */
        void disconnection();

        /**
         * @brief Ajoute la réponse reçue en paramètre dans la liste des messages à envoyer.
         * @param reply Réponse à envoyer
         */
        void sendCommandReply(std::shared_ptr<commands::CommandReply> reply);
};

/** Callbacks FMOD pour le stream de musiques distantes **/

FMOD_RESULT F_CALLBACK openCallback(const char *fileName, unsigned int *filesize, void **handle, void *userdata);
FMOD_RESULT F_CALLBACK closeCallback(void *handle, void *userdata);
FMOD_RESULT F_CALLBACK readCallback(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata);
FMOD_RESULT F_CALLBACK seekCallback(void *handle, unsigned int pos, void *userdata);


} // network

#endif  // __PLAYERSOCKET_H__

