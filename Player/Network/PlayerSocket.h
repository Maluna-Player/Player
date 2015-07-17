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
#include <QTreeWidgetItem>
#include "RemoteSong.h"
#include "PlayerMessage.h"

class PlayerSocket : public QObject
{
    Q_OBJECT

    private:

        bool m_Connected;

        QTcpServer *mp_Server;
        QTcpSocket *mp_Socket;

        int m_NbSentListItems;
        QVector<RemoteSong*> mp_RemoteSongs;

        PlayerMessage *mp_SendMessage;
        PlayerMessage *mp_ReceiveMessage;

        QThread *mp_SocketThread;


        /**
         * @brief Cherche l'item correspondant au numéro passé en paramètre dans l'arborescence parent.
         * @param num Numéro de l'item à chercher
         * @param parent Racine de l'arborescence dans laquelle chercher
         * @return Item correspondant au numéro indiqué
         */
        virtual QTreeWidgetItem* getItem(int num, QTreeWidgetItem *parent) const;

        /**
         * @brief Envoie la liste des éléments récursivement à partir de item.
         * @param item Element racine à partir duquel envoyer la liste
         * @param parent Parent de item dans l'arborescence
         */
        virtual void sendSongs(QTreeWidgetItem *item, int parent);

        /**
         * @brief Lit les données des musiques distantes reçues sur le socket.
         * @return Liste des musiques reçues
         */
        virtual QList<QTreeWidgetItem*> readRemoteSongList();

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
         * @brief Envoie la liste des musiques.
         */
        virtual void startConnection();

        /**
         * @brief Affiche l'erreur de connexion.
         */
        virtual void error();

    signals:

        /**
         * @brief Signal émis lorsque la connexion entre les deux utilisateurs est établie.
         */
        void connected();

        /**
         * @brief Signal émis lors de la fin de la connexion avec un autre client.
         */
        void disconnected();

    public:

        PlayerSocket();
        virtual ~PlayerSocket();

        /**
         * @brief Met le socket serveur en écoute de clients.
         * @param address Adresse sur laquelle écouter les connexions entrantes
         */
        virtual void listen(QHostAddress address);

        /**
         * @brief Se connecte à l'hôte passé en paramètre par le port défini par l'application.
         * @param address Adresse de l'hôte
         */
        virtual void connectToHost(const QString& address);

        /**
         * @brief Envoie au client la liste des musiques enregistrées et reçoit ses musiques.
         * @param songs Arborescence des musiques
         * @param nbSons Nombre de musiques à envoyer
         * @return Arborescence des musiques de l'autre client
         */
        virtual QList<QTreeWidgetItem*> exchangeSongList(const QList<QTreeWidgetItem*>& songs, int nbSongs);
};

#endif  // __PLAYERSOCKET_H__

