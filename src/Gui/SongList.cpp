/*************************************
 * @file    SongList.cpp
 * @date    12/09/14
 * @author  Manuel
 *
 * Définitions de la classe SongList.
 *************************************
*/

#include "SongList.h"
#include "Constants.h"
#include "../Audio/Song.h"
#include "../Util/Tools.h"
#include "../Exceptions/FileLoadingException.h"
#include "../Exceptions/ArrayAccessException.h"
#include <QScrollBar>
#include <QMouseEvent>


namespace gui {


SongList::SongList(QWidget *parent)
    : QTreeWidget(parent),
      m_DeleteIcon(util::Tools::loadImage(QString(IMAGES_SUBDIR) + "delete.png").scaledToWidth(14)),
      m_BrokenIcon(util::Tools::loadImage(QString(IMAGES_SUBDIR) + "broken_file.png")),
      mp_CurrentSong(nullptr), mp_PreviousHilightedItem(nullptr)
{
    QPalette p(palette());
    p.setColor(QPalette::Base, QColor(24, 0, 96));
    setPalette(p);

    setStyleSheet("QTreeWidget { color: rgb(212, 255, 250); border: 3px solid rgb(25, 25, 25); }");

    setColumnCount(3);
    setHeaderHidden(true);
    setMouseTracking(true);
    setColumnWidth(0, 170);
    setColumnWidth(1, 90);
    setColumnWidth(2, 15);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedWidth(LIST_WIDTH);

    SongListItem *localSongsItem = new SongListItem(SongListItem::ElementType::ROOT, "Mes musiques");
    SongListItem *remoteSongsItem = new SongListItem(SongListItem::ElementType::ROOT, "Musiques distantes");

    addTopLevelItem(localSongsItem);
    addTopLevelItem(remoteSongsItem);

    connect(this, &QTreeWidget::itemClicked, this, &SongList::onItemClicked);
}

// ==============================
// ==============================

SongListItem* SongList::getRootNode(SongList_t list) const
{
    if (list & SongList_t::LOCAL_SONGS)
        return static_cast<SongListItem*>(topLevelItem(0));
    else
        return static_cast<SongListItem*>(topLevelItem(1));
}

// ==============================
// ==============================

unsigned int SongList::setSongDetails(SongListItem *item) const
{
    auto parentDepth = item->parent()->getDepth();
    item->setDepth(parentDepth + 1);

    if (item->isSong())
    {
        std::shared_ptr<audio::Song> song = item->getAttachedSong();
        if (song)
        {
            item->setText(0, song->getTitle());
            item->setLength(song->getLength());

            return song->getLength();
        }
    }
    else
    {
        unsigned int lengthSum = 0;

        for (int i = 0; i < item->childCount(); ++i)
            lengthSum += setSongDetails(static_cast<SongListItem*>(item->child(i)));

        item->setLength(lengthSum);
        return lengthSum;
    }

    return 0;
}

// ==============================
// ==============================

void SongList::addChildSong(SongListItem *item, SongListItem *parent, int pos) const
{
    if (pos < 0)
        parent->addChild(item);
    else
        parent->insertChild(pos, item);

    const auto itemLength = setSongDetails(item);
    parent->setLength(parent->getLength() + itemLength);

    while (!parent->isRoot() && parent->parent())
    {
        parent = parent->parent();
        parent->setLength(parent->getLength() + itemLength);
    }
}

// ==============================
// ==============================

void SongList::removeSong(SongListItem *item)
{
    if (mp_PreviousHilightedItem == item)
        mp_PreviousHilightedItem = nullptr;

    if (mp_CurrentSong == item)
        mp_CurrentSong = nullptr;

    SongListItem *currentItem = item;
    SongListItem *parent = currentItem->parent();
    const auto itemLength = currentItem->getLength();

    parent->removeChild(currentItem);
    parent->setLength(parent->getLength() - itemLength);
    delete currentItem;

    while (parent->parent())
    {
        currentItem = parent;
        parent = parent->parent();
        parent->setLength(parent->getLength() - itemLength);

        if (currentItem->childCount() == 0)
        {
            parent->removeChild(currentItem);
            delete currentItem;
        }
    }
}

// ==============================
// ==============================

void SongList::removeSong(const SongListIterator& it)
{
    removeSong(*it);
}

// ==============================
// ==============================

void SongList::clearList(SongList_t list)
{
    SongListItem *root = getRootNode(list);
    SongListIterator it(root);

    while (!it.isNull())
    {
        if ((*it)->isSong())
        {
            std::shared_ptr<audio::Song> song = (*it)->getAttachedSong();
            if (song && (list == SongList_t::LOCAL_SONGS || list == SongList_t::REMOTE_SONGS
                         || (list == SongList_t::DIRECTORY_SONGS && song->isInFolder())
                         || (list == SongList_t::IMPORTED_SONGS && !song->isInFolder())))
                removeSong(it);
            else
                ++it;
        }
        else
            ++it;
    }
}

// ==============================
// ==============================

void SongList::setCurrentSong(audio::Player::SongId songId)
{
    if (mp_CurrentSong != nullptr)
        mp_CurrentSong->setTextColor(QColor(212, 255, 250));

    SongListIterator it(this, QTreeWidgetItemIterator::Selectable);

    while (!it.isNull())
    {
        std::shared_ptr<audio::Song> song = (*it)->getAttachedSong();
        if (song && song->isAvailable())
        {
            if (song->getId() == songId)
            {
                mp_CurrentSong = *it;
                mp_CurrentSong->setTextColor(QColor(21, 191, 221));
                return;
            }
        }

        ++it;
    }

    mp_CurrentSong = nullptr;
}

// ==============================
// ==============================

void SongList::onItemClicked(QTreeWidgetItem *item, int column)
{
    SongListItem *songItem = static_cast<SongListItem*>(item);

    if (songItem->isSong())
    {
        std::shared_ptr<audio::Song> song = songItem->getAttachedSong();

        if (column == 2)
        {
            removeSong(songItem);

            if (song)
                emit songRemoved(song->getId());
        }
        else if (song)
            emit songPressed(song->getId());
    }
}

// ==============================
// ==============================

void SongList::mouseMoveEvent(QMouseEvent *event)
{
    QTreeWidgetItem *flewItem = itemAt(event->x(), event->y());

    if (mp_PreviousHilightedItem != nullptr)
    {
        mp_PreviousHilightedItem->setIcon(2, QIcon());
        mp_PreviousHilightedItem = nullptr;
    }

    if (flewItem)
    {
        SongListItem *item = static_cast<SongListItem*>(flewItem);

        if (item->isSong())
        {
            item->setIcon(2, m_DeleteIcon);
            mp_PreviousHilightedItem = item;
        }
    }

    QTreeWidget::mouseMoveEvent(event);
}

// ==============================
// ==============================

void SongList::leaveEvent(QEvent * /*event*/)
{
    if (mp_PreviousHilightedItem != nullptr)
    {
        mp_PreviousHilightedItem->setIcon(2, QIcon());
        mp_PreviousHilightedItem = nullptr;
    }
}

// ==============================
// ==============================

SongTreeRoot* SongList::getSongHierarchy(SongList_t list) const
{
    return static_cast<SongListItem*>(getRootNode(list));
}

// ==============================
// ==============================

void SongList::addSong(SongList_t list, SongListItem *item)
{
    SongListItem *root = getRootNode(list);

    if (root)
        addChildSong(item, root);
}

// ==============================
// ==============================

void SongList::addTree(SongList_t list, SongTreeRoot *songs)
{
    SongListItem *root = getRootNode(list);
    if (root)
    {
        QList<QTreeWidgetItem*> items = songs->takeChildren();
        int pos = 0;

        for (auto item : items)
        {
            if (list == SongList_t::DIRECTORY_SONGS)
                addChildSong(static_cast<SongListItem*>(item), root, pos);
            else
                addChildSong(static_cast<SongListItem*>(item), root);

            ++pos;
        }

        root->setExpanded(true);
    }

    delete songs;
}

// ==============================
// ==============================

void SongList::disableSong(audio::Player::SongId songId)
{
    SongListIterator it(this, QTreeWidgetItemIterator::Selectable);

    while (!it.isNull())
    {
        std::shared_ptr<audio::Song> song = (*it)->getAttachedSong();
        if (song && song->getId() == songId)
        {
            (*it)->setTextColor(QColor(255, 0, 0));
            (*it)->setIcon(0, QIcon(m_BrokenIcon));

            return;
        }

        ++it;
    }
}


} // gui
