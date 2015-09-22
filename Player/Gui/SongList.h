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
#include "SongListItem.h"

class SongList : public QTreeWidget
{
    Q_OBJECT

    private:

        int m_CurrentSong;

    protected:

        virtual void mousePressEvent(QMouseEvent *event);

    signals:

        /**
         * @brief Signal émis lorsque l'utilisateur clique sur une musique locale de la liste.
         * @param song Musique sélectionnée
         */
        void songPressed(int song);

    public:

        enum SongList_t { LOCAL_SONGS, REMOTE_SONGS };

        SongList(QWidget *parent = 0);
        virtual ~SongList();

        /**
         * @brief Vide la liste du noeud passé en paramètre.
         * @param list Liste à nettoyer (locale/distante)
         */
        virtual void clearList(SongList_t list = LOCAL_SONGS);

        /**
         * @brief Change le son marqué comme son courant dans la liste.
         * @param songNum Indice de la musique actuelle
        */
        virtual void setCurrentSong(int songNum);

        /**
         * @brief getSongHierarchy
         * @param list Liste à retourner
         * @return Arborescence de musiques de la liste.
         */
        virtual SongTreeRoot* getSongHierarchy(SongList_t list = LOCAL_SONGS) const;

        /**
         * @brief Ajoute les titres et durées de la liste passée en paramètre.
         * @param list Type de liste dans laquelle ajouter les musiques (locales, distantes)
         * @param songs Liste de sons à ajouter
        */
        virtual void add(SongList_t list, SongTreeRoot *songs);
};

#endif  // __SONGLIST_H__
