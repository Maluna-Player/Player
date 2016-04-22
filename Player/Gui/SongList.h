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
#include "SongListIterator.h"
#include "Constants.h"
#include "../Audio/Player.h"


namespace gui {


class SongList : public QTreeWidget
{
    Q_OBJECT

    private:

        int m_CurrentSong;


        /**
         * @brief Récupère la racine correspondant à la liste passée en paramètre.
         * @param list Liste dont on veut récupérer la racine
         * @return Noeud racine
         */
        virtual SongListItem* getRootNode(Constants::SongList list) const;

        /**
         * @brief Supprime de la liste l'élément passé en paramètre (avec ses parents récursivement s'il s'agit du seul fils).
         * @param it Itérateur sur l'élément à supprimer
         */
        virtual void removeSong(const SongListIterator& it);

    protected:

        virtual void mousePressEvent(QMouseEvent *event) override;

    signals:

        /**
         * @brief Signal émis lorsque l'utilisateur clique sur une musique locale de la liste.
         * @param song Musique sélectionnée
         */
        void songPressed(audio::Player::SongId song);

    public:

        SongList(QWidget *parent = nullptr);
        virtual ~SongList();

        /**
         * @brief Vide la liste du noeud passé en paramètre.
         * @param list Liste à nettoyer (locale/distante)
         */
        virtual void clearList(Constants::SongList list = Constants::LOCAL_SONGS);

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
        virtual SongTreeRoot* getSongHierarchy(Constants::SongList list = Constants::LOCAL_SONGS) const;

        /**
         * @brief Ajoute le son passé en paramètre dans la liste.
         * @param list Type de liste dans laquelle ajouter la musique
         * @param item Elément à ajouter
         */
        virtual void addSong(Constants::SongList list, SongListItem *item);

        /**
         * @brief Ajoute les titres et durées de la liste passée en paramètre.
         * @param list Type de liste dans laquelle ajouter les musiques (locales, distantes)
         * @param songs Liste de sons à ajouter
        */
        virtual void addTree(Constants::SongList list, SongTreeRoot *songs);
};


} // gui

#endif  // __SONGLIST_H__
