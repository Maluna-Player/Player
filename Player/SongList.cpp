/*************************************
 * @file    SongList.cpp
 * @date    12/09/14
 * @author  Manuel
 *
 * Définitions de la classe SongList.
 *************************************
*/

#include "SongList.h"
#include "Song.h"
#include "Constants.h"
#include "FileLoadingException.h"
#include "ArrayAccessException.h"
#include "Tools.h"
#include <QScrollBar>


SongList::SongList(QWidget *parent) : QTreeWidget(parent), m_Selected(-1)
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
}

// ==============================
// ==============================

SongList::~SongList()
{

}

// ==============================
// ==============================

void SongList::setCurrentSong(int song)
{
    /*if (m_Selected != -1)
    {
        if (m_Selected >= m_SongDetails.size())
            throw ArrayAccesException("SongList::setCurrentSong", m_SongDetails.size(), m_Selected);

        m_SongDetails.at(m_Selected).first.setColor(sf::Color::White);
        m_SongDetails.at(m_Selected).second.setColor(sf::Color::White);
    }

    if (song >= m_SongDetails.size())
        throw ArrayAccesException("SongList::setCurrentSong", m_SongDetails.size(), song);

    m_Selected = song;
    m_SongDetails.at(m_Selected).first.setColor(sf::Color(21, 191, 221));
    m_SongDetails.at(m_Selected).second.setColor(sf::Color(21, 191, 221));*/
}

// ==============================
// ==============================

void SongList::displaySongs(QTreeWidgetItem *item) const
{
    if (item->childCount() > 0)
    {
        for (int i = 0; i < item->childCount(); i++)
            displaySongs(item->child(i));
    }
    else
    {
        Song *song = reinterpret_cast<Song*>(item->data(0, Qt::UserRole).value<quintptr>());
        if (song)
        {
            item->setText(0, song->getTitle());
            item->setText(1, Tools::msToString(song->getLength()));
        }
    }
}

// ==============================
// ==============================

void SongList::add(const QList<QTreeWidgetItem*>& songs)
{
    addTopLevelItems(songs);

    for (int i = 0; i < topLevelItemCount(); i++)
        displaySongs(topLevelItem(i));
}
