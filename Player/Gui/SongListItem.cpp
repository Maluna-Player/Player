/*************************************
 * @file    SongListItem.cpp
 * @date    19/09/15
 * @author  Manuel
 *
 * Définitions de la classe SongListItem.
 *************************************
*/

#include "SongListItem.h"
#include "../Audio/Song.h"


unsigned int SongListItem::cpt = 0;

SongListItem::SongListItem(ElementType_t type, SongListItem *parent, const QString& name)
    : QTreeWidgetItem(), m_Type(type), mp_AttachedSong(0)
{
    Qt::ItemFlags flags = Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
    if (type == SONG)
        flags |= Qt::ItemIsSelectable;

    setFlags(flags);
    setParent(parent);
    setText(0, name);

    cpt++;
    m_Num = cpt;
}

// ==============================
// ==============================

SongListItem::~SongListItem()
{

}

// ==============================
// ==============================

bool SongListItem::isRoot() const
{
    return (m_Type == ROOT);
}

// ==============================
// ==============================

bool SongListItem::isSong() const
{
    return (m_Type == SONG);
}

// ==============================
// ==============================

unsigned int SongListItem::getNum() const
{
    return m_Num;
}

// ==============================
// ==============================

SongListItem* SongListItem::parent() const
{
    return static_cast<SongListItem*>(QTreeWidgetItem::parent());
}

// ==============================
// ==============================

unsigned int SongListItem::getParentNum() const
{
    return (parent()) ? parent()->getNum() : 0;
}

// ==============================
// ==============================

Song* SongListItem::getAttachedSong() const
{
    return mp_AttachedSong;
}

// ==============================
// ==============================

void SongListItem::setParent(SongListItem *parent)
{
    if (parent)
        parent->addChild(this);
}

// ==============================
// ==============================

void SongListItem::setAttachedSong(Song *song)
{
    mp_AttachedSong = song;
}

// ==============================
// ==============================

QByteArray SongListItem::toPacket() const
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out << static_cast<quint16>(m_Num);             // Numéro de l'item
    out << static_cast<quint16>(getParentNum());    // Parent de l'item (0 si racine)
    out << text(0);                                 // Nom de l'objet

    if (!isSong())                                  // Type de l'objet (dossier/musique)
        out << static_cast<quint8>(0);
    else
    {
        out << static_cast<quint8>(1);

        Song *song = getAttachedSong();
        if (song)
        {
            out << static_cast<quint32>(song->getLength()); // Durée de la musique
            out << song->getArtist();                       // Artiste de la musique
        }
    }

    return packet;
}
