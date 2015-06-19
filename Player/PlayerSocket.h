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
#include <QTreeWidgetItem>
#include "SendingThread.h"

class PlayerSocket : public QObject
{
    Q_OBJECT

    private:

        bool m_Connected;

        QTcpServer *mp_Server;
        QTcpSocket *mp_Socket;

        quint16 messageSize;

        SendingThread *mp_SendingThread;

        int nbRecievedSongs;
        int nbSongsToRecieve;
        QTreeWidgetItem *mp_RecievedSongs;


        /**
         * @brief Cherche l'item correspondant au numéro passé en paramètre.
         * @param num Numéro de l'item à chercher
         * @param parent Racine de l'arborescence dans laquelle chercher
         * @return Item correspondant au numéro indiqué
         */
        virtual QTreeWidgetItem* getItem(int num, QTreeWidgetItem *parent = 0) const;

    private slots:

        /**
         * @brief Se connecte avec le client demandé.
         */
        virtual void clientConnexion();

        /**
         * @brief Ferme le socket et signale à l'application la fin de la communication.
         */
        virtual void disconnection();

        /**
         * @brief Libère le socket client et signale à l'application la fin de la communication.
         */
        virtual void clientDisconnection();

        /**
         * @brief Envoie la liste des musiques.
         */
        virtual void startConnection();

        /**
         * @brief Affiche l'erreur de connexion.
         */
        virtual void error();

        /**
         * @brief Lit les données des musiques distantes reçues sur le socket.
         */
        virtual void readRemoteSongList();

    signals:

        /**
         * @brief Signal émis lorsque la connexion entre les deux utilisateurs est établie.
         */
        void connected();

        /**
         * @brief Signal émis lors de la fin de la connexion avec un autre client.
         */
        void endedConnection();

        /**
         * @brief Signal émis lorsque l'ensemble de la liste des musiques distantes a été reçue.
         * @param Liste des musiques reçue
         */
        void songsRecieved(QList<QTreeWidgetItem*>);

    public:

        PlayerSocket();
        virtual ~PlayerSocket();

        /**
         * @brief Met le socket serveur en écoute de clients.
         * @param address Adresse sur laquelle écouter les connexions entrantes
         */
        virtual void listen(const QHostAddress& address);

        /**
         * @brief Se connecte à l'hôte passé en paramètre par le port défini par l'application.
         * @param address Adresse de l'hôte
         */
        virtual void connectToHost(const QString& address);

        /**
         * @brief Envoie au client la liste des musiques enregistrées.
         * @param songs Arborescence des musiques
         * @param nbSons Nombre de musiques à envoyer
         */
        virtual void send(const QList<QTreeWidgetItem*>& songs, int nbSongs);
};

#endif  // __PLAYERSOCKET_H__

