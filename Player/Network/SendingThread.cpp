/*************************************
 * @file    SendingThread.cpp
 * @date    17/06/14
 * @author  Manuel
 *
 * Définitions de la classe SendingThread.
 *************************************
*/

#include "SendingThread.h"


SendingThread::SendingThread(QTcpSocket *socket, const QList<QTreeWidgetItem*>& songList, QWidget *parent)
    : QThread(parent), mp_Socket(socket), nbSentItems(0), mp_Songs(songList)
{

}

// ==============================
// ==============================

SendingThread::~SendingThread()
{

}

// ==============================
// ==============================

void SendingThread::sendSongs(QTreeWidgetItem *item, int parent)
{
    int num = nbSentItems + 1;

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out << static_cast<quint16>(0);             // Taille du paquet
    out << static_cast<quint16>(num);           // Numéro de l'item
    out << static_cast<quint16>(parent);        // Parent de l'item (0 si racine)
    out << item->text(0);                       // Nom de l'objet

    if (item->childCount() > 0)
        out << static_cast<quint8>(0);          // Type de l'objet (dossier/musique)
    else
    {
        out << static_cast<quint8>(1);          // Type de l'objet (dossier/musique)

        Song *song = reinterpret_cast<Song*>(item->data(0, Qt::UserRole).value<quintptr>());
        if (song)
        {
            out << static_cast<quint32>(song->getLength());
        }
    }

    out.device()->seek(0);
    out << static_cast<quint16>(packet.size() - sizeof(quint16));

    mp_Socket->write(packet);

    nbSentItems++;

    for (int i = 0; i < item->childCount(); i++)
        sendSongs(item->child(i), num);
}

// ==============================
// ==============================

void SendingThread::run()
{
    for (int i = 0; i < mp_Songs.size(); i++)
        sendSongs(mp_Songs.at(i), 0);
}
