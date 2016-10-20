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
#include "../Util/Tools.h"


namespace gui {


unsigned int SongListItem::m_Cpt = 0;

// ==============================
// ==============================

SongListItem::SongListItem(ElementType type, SongListItem *parent, const QString& name)
    : QTreeWidgetItem(), m_Type(type), m_Length(0), m_Depth(0), mp_AttachedSong(nullptr)
{
    Qt::ItemFlags flags = Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
    if (type == ElementType::SONG)
        flags |= Qt::ItemIsSelectable;

    setFlags(flags);
    setParent(parent);
    setText(0, name);
    setText(1, util::Tools::msToString(m_Length));

    if (type == ElementType::ROOT || type == ElementType::DIRECTORY)
    {
        setIcon(0, QIcon(util::Tools::loadImage(QString(IMAGES_SUBDIR) + "folder.png")));

        QFont lengthFont = font(1);
        lengthFont.setBold(true);
        setFont(1, lengthFont);
    }

    m_Cpt++;
    m_Num = m_Cpt;
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
    return (m_Type == ElementType::ROOT);
}

// ==============================
// ==============================

bool SongListItem::isSong() const
{
    return (m_Type == ElementType::SONG);
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

std::shared_ptr<audio::Song> SongListItem::getAttachedSong() const
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

void SongListItem::setAttachedSong(std::shared_ptr<audio::Song> song)
{
    mp_AttachedSong = song;
}

// ==============================
// ==============================

void SongListItem::setLengthText()
{
    QString lengthText{};

    for (unsigned int i = 0; i < m_Depth; ++i)
        lengthText += "  ";

    lengthText += "|- " + util::Tools::msToString(m_Length);
    setText(1, lengthText);
}

// ==============================
// ==============================

unsigned int SongListItem::getLength() const
{
    return m_Length;
}

// ==============================
// ==============================

void SongListItem::setLength(unsigned int length)
{
    m_Length = length;
    setLengthText();
}

// ==============================
// ==============================

unsigned int SongListItem::getDepth() const
{
    return m_Depth;
}

// ==============================
// ==============================

void SongListItem::setDepth(unsigned int depth)
{
    m_Depth = depth;
    setLengthText();
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

        std::shared_ptr<audio::Song> song = getAttachedSong();
        if (song)
        {
            out << static_cast<quint16>(song->getId());     // Id de la musique
            out << static_cast<quint32>(song->getLength()); // Durée de la musique
            out << song->getArtist();                       // Artiste de la musique
        }
    }

    return packet;
}


} // gui
