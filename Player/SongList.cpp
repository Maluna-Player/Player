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
#include "FileLoadingException.h"
#include "ArrayAccessException.h"
#include "Tools.h"
#include <QScrollBar>


SongList::SongList(QWidget *parent) : QTreeWidget(parent), m_Selected(-1)
{
    QPalette p(palette());
    p.setColor(QPalette::Base, QColor(50, 50, 50));
    setPalette(p);

    setStyleSheet("border: 3px solid rgb(25, 25, 25);");
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

void SongList::add(const QString& title, const QString& length)
{
    QStringList songDetails;
    songDetails << title << length;

    QTreeWidgetItem *item = new QTreeWidgetItem(songDetails);
    item->setTextColor(0, Qt::white);
    item->setTextColor(1, Qt::white);
    addTopLevelItem(item);
}

// ==============================
// ==============================

void SongList::add(const QVector<QPair<QString, int> >& songs)
{
    for (int i = 0; i < songs.size(); i++)
        add(songs.at(i).first, Tools::msToString(songs.at(i).second));
}
