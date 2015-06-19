/*************************************
 * @file    SendingThread.h
 * @date    17/06/15
 * @author  Manuel
 *
 * Déclarations de la classe SendingThread
 * chargée d'envoyer la liste des musiques.
 *************************************
*/

#ifndef __SENDINGTHREAD_H__
#define __SENDINGTHREAD_H__

#include <QThread>
#include <QTcpSocket>
#include <QTreeWidgetItem>
#include "Song.h"

class SendingThread : public QThread
{
    private:

        QTcpSocket *mp_Socket;

        int nbSentItems;

        QList<QTreeWidgetItem*> mp_Songs;


        /**
         * @brief Envoie la liste des éléments récursivement à partir de item.
         * @param item Element racine à partir duquel envoyer la liste
         * @param parent Parent de item dans l'arborescence
         */
        virtual void sendSongs(QTreeWidgetItem *item, int parent);

    protected:

        virtual void run();

    public:

        SendingThread(QTcpSocket *socket, const QList<QTreeWidgetItem*>& songList, QWidget *parent = 0);
        virtual ~SendingThread();
};

#endif  // __SENDINGTHREAD_H__

