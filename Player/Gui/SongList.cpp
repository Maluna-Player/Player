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
#include "Tools.h"
#include <QScrollBar>
#include <QMouseEvent>


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
    setFixedWidth(260);

    SongListItem *localSongsItem = new SongListItem(SongListItem::ROOT, 0, "Mes musiques");
    SongListItem *remoteSongsItem = new SongListItem(SongListItem::ROOT, 0, "Musiques distantes");

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

void SongList::clearList(SongList_t list)
{
    QTreeWidgetItem *root = topLevelItem(list);
    root->takeChildren();
}

// ==============================
// ==============================

void SongList::setCurrentSong(int songNum)
{
    QTreeWidgetItemIterator it(this, QTreeWidgetItemIterator::Selectable);

    while (*it)
    {
        Song *song = reinterpret_cast<Song*>((*it)->data(0, Qt::UserRole).value<quintptr>());
        if (song)
        {
            if (song->getNum() == m_CurrentSong)
            {
                (*it)->setTextColor(0, QColor(212, 255, 250));
                (*it)->setTextColor(1, QColor(212, 255, 250));
            }

            if (song->getNum() == songNum)
            {
                (*it)->setTextColor(0, QColor(21, 191, 221));
                (*it)->setTextColor(1, QColor(21, 191, 221));
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
        Song *song = reinterpret_cast<Song*>(selectedItem->data(0, Qt::UserRole).value<quintptr>());
        if (song)
            emit songPressed(song->getNum());
    }

    QTreeWidget::mousePressEvent(event);
}

// ==============================
// ==============================

SongTreeRoot* SongList::getSongHierarchy(SongList_t list) const
{
    return static_cast<SongListItem*>(topLevelItem(list));
}

// ==============================
// ==============================

void SongList::add(SongList_t list, SongTreeRoot *songs)
{
    QTreeWidgetItem *root = topLevelItem(list);
    if (root)
    {
        root->addChildren(songs->takeChildren());

        QTreeWidgetItemIterator it(root);
        int nodesCount = 1;

        for (int i = 0; i < nodesCount; i++)
        {
            if (static_cast<SongListItem*>(*it)->isSong())
            {
                Song *song = reinterpret_cast<Song*>((*it)->data(0, Qt::UserRole).value<quintptr>());
                if (song)
                {
                    (*it)->setText(0, song->getTitle());
                    (*it)->setText(1, Tools::msToString(song->getLength()));
                }
            }

            nodesCount += (*it)->childCount();
            ++it;
        }

        root->setExpanded(true);
    }

    delete songs;
}
