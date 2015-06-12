/*************************************
 * @file    SongList.h
 * @date    12/09/14
 * @author  Manuel
 *
 * Déclarations de la classe SongList
 * comprenant l'affichage de la liste
 * des sons ouverts dans le player.
 *************************************
*/

#ifndef __SONGLIST_H__
#define __SONGLIST_H__

#include <QTreeWidget>
#include <QVector>
#include <QPair>
#include <QString>

class SongList : public QTreeWidget
{
    private:

        int m_Selected;

    public:

        SongList(QWidget *parent = 0);
        virtual ~SongList();

        /**
         * @brief Change le son marqué comme son courant dans la liste.
         * @param song Indice de la musique actuelle
        */
        virtual void setCurrentSong(int song);

        /**
         * @brief Affiche l'ensemble des informations dans la liste à partie de l'arborescence.
         * @param item Parent de l'arborescence à afficher
         */
        virtual void displaySongs(QTreeWidgetItem *item) const;

        /**
         * @brief Ajoute les titres et durées de la liste passée en paramètre.
         * @param songs Liste de sons à ajouter
        */
        virtual void add(const QList<QTreeWidgetItem*>& songs);
};

#endif  // __SONGLIST_H__
