/*************************************
 * @file    SongList.cpp
 * @date    12/09/14
 * @author  Manuel
 *
 * Définitions de la classe SongList.
 *************************************
*/

#include "SongList.h"
#include "../Audio/Song.h"
#include "../Constants.h"
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

    QTreeWidgetItem *localSongsItem = new QTreeWidgetItem;
    localSongsItem->setText(0, "Mes musiques");

    QTreeWidgetItem *remoteSongsItem = new QTreeWidgetItem;
    remoteSongsItem->setText(0, "Musiques distantes");

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

bool SongList::isSong(QTreeWidgetItem *item) const
{
    return (item->childCount() <= 0);
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

void SongList::setCurrentSong(SongList_t list, int songNum)
{
    QTreeWidgetItem *root = topLevelItem(list);
    QTreeWidgetItemIterator it(root);

    while (*it)
    {
        if (isSong(*it))
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

    if (selectedItem && isSong(selectedItem))
    {
        Song *song = reinterpret_cast<Song*>(selectedItem->data(0, Qt::UserRole).value<quintptr>());
        if (song && !song->isRemote())
            emit songPressed(song->getNum());
    }

    QTreeWidget::mousePressEvent(event);
}

// ==============================
// ==============================

const QList<QTreeWidgetItem*> SongList::getSongHierarchy(SongList_t list) const
{
    QTreeWidgetItem *root = topLevelItem(list);

    QList<QTreeWidgetItem*> children;

    for (int i = 0; i < root->childCount(); i++)
        children.append(root->child(i));

    return children;
}

// ==============================
// ==============================

void SongList::add(SongList_t list, const QList<QTreeWidgetItem*>& songs)
{
    QTreeWidgetItem *root = topLevelItem(list);
    root->addChildren(songs);

    QTreeWidgetItemIterator it(root);
    while (*it)
    {
        if (isSong(*it))
        {
            Song *song = reinterpret_cast<Song*>((*it)->data(0, Qt::UserRole).value<quintptr>());
            if (song)
            {
                (*it)->setText(0, song->getTitle());
                (*it)->setText(1, Tools::msToString(song->getLength()));
            }
        }

        ++it;
    }

    root->setExpanded(true);
}
