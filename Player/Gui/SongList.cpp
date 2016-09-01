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
#include "../Exceptions/FileLoadingException.h"
#include "../Exceptions/ArrayAccessException.h"
#include <QScrollBar>
#include <QMouseEvent>


namespace gui {


SongList::SongList(QWidget *parent) : QTreeWidget(parent), m_CurrentSong(-1)
{
    QPalette p(palette());
    p.setColor(QPalette::Base, QColor(24, 0, 96));
    setPalette(p);

    setStyleSheet("color: rgb(212, 255, 250); border: 3px solid rgb(25, 25, 25);");
    verticalScrollBar()->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop: 0  rgb(200, 150, 130), stop: 0.5 rgb(150, 47, 130),  stop:1 rgb(200, 30, 150));");

    setColumnCount(2);
    setHeaderHidden(true);
    setColumnWidth(0, 180);
    resizeColumnToContents(1);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedWidth(280);

    SongListItem *localSongsItem = new SongListItem(SongListItem::ElementType::ROOT, nullptr, "Mes musiques");
    SongListItem *remoteSongsItem = new SongListItem(SongListItem::ElementType::ROOT, nullptr, "Musiques distantes");

    addTopLevelItem(localSongsItem);
    addTopLevelItem(remoteSongsItem);
}

// ==============================
// ==============================

SongList::~SongList()
{

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

void SongList::setDepth(SongListItem *item, int depth) const
{
    QString shift{};

    for (int i = 0; i < depth; ++i)
        shift += "  ";

    shift += "|- ";
    item->setText(1, shift + item->text(1));

    if (!item->isSong())
    {
        for (int i = 0; i < item->childCount(); ++i)
            setDepth(static_cast<SongListItem*>(item->child(i)), depth + 1);
    }
}

// ==============================
// ==============================

void SongList::addChildSong(SongListItem *item, SongListItem *parent) const
{
    parent->addChild(item);

    const auto itemLength = setSongDetails(item);
    parent->setLength(parent->getLength() + itemLength);

    unsigned int depth = 1;

    while (!parent->isRoot() && parent->parent())
    {
        parent = parent->parent();
        parent->setLength(parent->getLength() + itemLength);

        ++depth;
    }

    setDepth(item, depth);
}

// ==============================
// ==============================

void SongList::removeSong(const SongListIterator& it) const
{
    SongListItem *parent = (*it)->parent();
    parent->removeChild(*it);

    while (!parent->isRoot() && parent->parent() && parent->childCount() == 0)
    {
        SongListItem *dirToRemove = parent;
        parent = parent->parent();
        parent->removeChild(dirToRemove);
        delete dirToRemove;
    }
}

// ==============================
// ==============================

void SongList::clearList(SongList_t list)
{
    SongListItem *root = getRootNode(list);

    if (list == SongList_t::LOCAL_SONGS || list == SongList_t::REMOTE_SONGS)
        root->takeChildren();
    else if (list == SongList_t::DIRECTORY_SONGS || list == SongList_t::IMPORTED_SONGS)
    {
        SongListIterator it(root);

        while (!it.isNull())
        {
            if ((*it)->isSong())
            {
                std::shared_ptr<audio::Song> song = (*it)->getAttachedSong();
                if (song && ((list == SongList_t::DIRECTORY_SONGS && song->isInFolder())
                              || (list == SongList_t::IMPORTED_SONGS && !song->isInFolder())))
                    removeSong(it);
                else
                    ++it;
            }
            else
                ++it;
        }
    }
}

// ==============================
// ==============================

void SongList::setCurrentSong(int songNum)
{
    SongListIterator it(this, QTreeWidgetItemIterator::Selectable);

    while (!it.isNull())
    {
        std::shared_ptr<audio::Song> song = (*it)->getAttachedSong();
        if (song)
        {
            if (song->getNum() == m_CurrentSong)
            {
                (*it)->setForeground(0, QColor(212, 255, 250));
                (*it)->setForeground(1, QColor(212, 255, 250));
            }

            if (song->getNum() == songNum)
            {
                (*it)->setForeground(0, QColor(21, 191, 221));
                (*it)->setForeground(1, QColor(21, 191, 221));
            }
        }

        ++it;
    }

    m_CurrentSong = songNum;
}

// ==============================
// ==============================

void SongList::mousePressEvent(QMouseEvent *event)
{
    QTreeWidgetItem *selectedItem = itemAt(event->x(), event->y());

    if (selectedItem && static_cast<SongListItem*>(selectedItem)->isSong())
    {
        std::shared_ptr<audio::Song> song = static_cast<SongListItem*>(selectedItem)->getAttachedSong();
        if (song)
            emit songPressed(song->getNum());
    }

    QTreeWidget::mousePressEvent(event);
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
        for (auto item : items)
            addChildSong(static_cast<SongListItem*>(item), root);

        root->setExpanded(true);
    }

    delete songs;
}


} // gui
