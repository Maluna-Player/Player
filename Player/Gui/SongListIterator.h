/*************************************
 * @file    SongListIterator.h
 * @date    26/09/15
 * @author  Manuel
 *
 * Déclarations de la classe SongListIterator
 * définissant un itérateur sur la liste de musiques.
 *************************************
*/

#ifndef __SONGLISTITERATOR__
#define __SONGLISTITERATOR__

#include <QTreeWidgetItemIterator>
#include "SongListItem.h"


namespace gui {


class SongListIterator : public QTreeWidgetItemIterator
{
    private:

        unsigned int m_NodesCpt;

        unsigned int m_NodesCount;


        /**
         * @brief Détermine si le parcours actuel se fait sur l'ensemble de l'arbre ou sur un sous-arbre.
         * @return true si la visite se fait sur un sous-arbre
         */
        bool isVisitingNodesTree() const;

    public:

        SongListIterator(QTreeWidget *widget, IteratorFlags flags = All);
        SongListIterator(QTreeWidgetItem *item);
        virtual ~SongListIterator() = default;

        /**
         * @brief Détermine s'il existe un prochain élément, et si le parcours effectué est terminé ou non.
         * @return true s'il n'y a plus d'élément courant
         */
        bool isNull() const;

        SongListItem* operator*() const;

        SongListIterator& operator++();
};


} // gui

#endif  // __SONGLISTITERATOR__

