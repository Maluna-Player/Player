/*************************************
 * @file    SongListIterator.cpp
 * @date    26/09/15
 * @author  Manuel
 *
 * Définitions de la classe SongListIterator.
 *************************************
*/

#include "SongListIterator.h"
#include <QTreeWidgetItem>


namespace gui {


SongListIterator::SongListIterator(QTreeWidget *widget, IteratorFlags flags)
    : QTreeWidgetItemIterator(widget, flags), m_NodesCpt(0), m_NodesCount(0)
{

}

// ==============================
// ==============================

SongListIterator::SongListIterator(QTreeWidgetItem *item)
    : QTreeWidgetItemIterator(item), m_NodesCpt(0), m_NodesCount(1)
{

}

// ==============================
// ==============================

SongListIterator::~SongListIterator()
{

}

// ==============================
// ==============================

bool SongListIterator::isVisitingNodesTree() const
{
    return m_NodesCount;
}

// ==============================
// ==============================

bool SongListIterator::isNull() const
{
    if (!isVisitingNodesTree())
        return !(**this);
    else
        return !(**this && m_NodesCpt < m_NodesCount);
}

// ==============================
// ==============================

SongListItem* SongListIterator::operator*() const
{
    return static_cast<SongListItem*>(QTreeWidgetItemIterator::operator*());
}

// ==============================
// ==============================

SongListIterator& SongListIterator::operator++()
{
    QTreeWidgetItem *currentItem = **this;

    if (isVisitingNodesTree())
        m_NodesCount += currentItem->childCount();

    QTreeWidgetItemIterator::operator++();
    m_NodesCpt++;

    return *this;
}


} // gui
