/*************************************
 * @file    PlayerMessageBox.h
 * @date    19/06/15
 * @author  Manuel
 *
 * Déclarations de la classe PlayerMessageBox
 * contenant la gestion d'envoi/réception
 * via le socket de communication.
 *************************************
*/

#ifndef __PLAYERMESSAGEBOX_H__
#define __PLAYERMESSAGEBOX_H__

#include <QTcpSocket>
#include <QVector>
#include <QMutex>
#include "Sendable.h"


namespace network {


class PlayerMessageBox : public QObject
{
    Q_OBJECT

    private:

        using MessageSize_t = quint32;

        QTcpSocket *mp_Socket;
        MessageSize_t m_MessageSize;

        bool m_IsSending;

        QVector<QByteArray> m_ReceivedMessages;
        QVector<QByteArray> m_MessagesToSend;

        QMutex m_ReceivingMutex;
        QMutex m_MessageGettingMutex;
        QMutex m_SendingMutex;


        /**
         * @brief Récupère le prochain message à envoyer dans la liste.
         * @return Message ou chaine vide si pas de messages
         */
        QByteArray getNextMessageToSend();

    private slots:

        /**
         * @brief Envoie l'ensemble des messages de la liste.
         */
        void sendMessages();

        /**
         * @brief Lit les messages reçus.
         */
        void receiveMessages();

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

        PlayerMessageBox(QTcpSocket *socket);
        virtual ~PlayerMessageBox() = default;

        /**
         * @brief Retourne le prochain message de la liste.
         * @return Message ou chaine vide si pas de messages
         */
        QByteArray getNextMessage();

        /**
         * @brief Attend le prochain message de la liste s'il n'est pas arrivé et le retourne.
         * @return Message suivant
         */
        QByteArray waitNextMessage();

        /**
         * @brief Ajoute le message passé en paramètre à la liste des messages à envoyer.
         * @param message Message à ajouter
         */
        void add(const QByteArray& message);

        /**
         * @brief Ajoute le contenu de l'objet passé en paramètre à la liste des messages à envoyer.
         * @param objectToSend Objet dont le contenu est à envoyer
         */
        void add(Sendable *objectToSend);
};


} // network

#endif  // __PLAYERMESSAGEBOX_H__

