/*************************************
 * @file    SongListItem.cpp
 * @date    02/07/15
 * @author  Manuel
 *
 * Définitions de la classe SongListItem.
 *************************************
*/

#include "SongListItem.h"
#include <QDataStream>


SongListItem::SongListItem(int num, const QString &title, int parent)
    : m_Num(num), m_Parent(parent), m_Title(title), m_Artist(""), m_File(false), m_Length(-1)
{

}

// ==============================
// ==============================

SongListItem::SongListItem(int num, int length, const QString& title, const QString& artist, int parent)
    : m_Num(num), m_Parent(parent), m_Title(title), m_Artist(artist), m_File(true), m_Length(length)
{

}

// ==============================
// ==============================

SongListItem::~SongListItem()
{

}

// ==============================
// ==============================

QByteArray SongListItem::toPacket() const
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out << static_cast<quint16>(m_Num);         // Numéro de l'item
    out << static_cast<quint16>(m_Parent);      // Parent de l'item (0 si racine)
    out << m_Title;                             // Nom de l'objet

    if (!m_File)                                // Type de l'objet (dossier/musique)
        out << static_cast<quint8>(0);
    else
    {
        out << static_cast<quint8>(1);
        out << static_cast<quint32>(m_Length);  // Durée de la musique
        out << m_Artist;                        // Artiste de la musique
    }

    return packet;
}
