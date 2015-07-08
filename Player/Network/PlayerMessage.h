/*************************************
 * @file    PlayerMessage.h
 * @date    19/06/15
 * @author  Manuel
 *
 * Déclarations de la classe PlayerMessage
 * contenant la gestion d'envoi/réception
 * via le socket de communication.
 *************************************
*/

#ifndef __PLAYERMESSAGE_H__
#define __PLAYERMESSAGE_H__

#include <QTcpSocket>
#include <QVector>
#include <QMutex>
#include "Sendable.h"

class PlayerMessage : public QObject
{
    Q_OBJECT

    private:

        QTcpSocket *mp_Socket;
        quint16 m_MessageSize;

        bool m_IsSending;

        QVector<QByteArray> m_Messages;
        QMutex m_Mutex;

    private slots:

        /**
         * @brief Envoie l'ensemble des messages de la liste.
         */
        virtual void sendMessages();

    public slots:

        /**
         * @brief Lit les messages reçus.
         */
        virtual void receive();

    signals:

        /**
         * @brief Signal émis lorsqu'un message est reçu.
         */
        void messageReceived();

        /**
         * @brief Signal émis lorsqu'un nouveau message est prêt à envoyer.
         */
        void newMessageToSend();

    public:

        PlayerMessage(QTcpSocket *socket);
        virtual ~PlayerMessage();

        /**
         * @brief Retourne le prochain message de la liste.
         * @return Message ou chaine vide si pas de messages
         */
        virtual QByteArray getNextMessage();

        /**
         * @brief Attend le prochain message de la liste s'il n'est pas arrivé et le retourne.
         * @return Message suivant
         */
        virtual QByteArray waitNextMessage();

        /**
         * @brief Ajoute le message passé en paramètre à la liste des messages à envoyer.
         * @param message Message à ajouter
         */
        virtual void add(const QByteArray& message);

        /**
         * @brief Ajoute le contenu de l'objet passé en paramètre à la liste des messages à envoyer.
         * @param objectToSend Objet dont le contenu est à envoyer
         */
        virtual void add(Sendable *objectToSend);
};

#endif  // __PLAYERMESSAGE_H__

