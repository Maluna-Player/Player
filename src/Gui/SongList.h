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
    Q_PROPERTY(int width READ width WRITE setFixedWidth)

    private:

        QPixmap m_DeleteIcon;

        QPixmap m_BrokenIcon;

        SongListItem *mp_CurrentSong;

        SongListItem *mp_PreviousHilightedItem;


        /**
         * @brief Récupère la racine correspondant à la liste passée en paramètre.
         * @param list Liste dont on veut récupérer la racine
         * @return Noeud racine
         */
        SongListItem* getRootNode(SongList_t list) const;

        /**
         * @brief Affiche le titre et le temps de l'élément passé en paramètre et de ses fils.
         * @param item Elément parent contenant les informations à afficher
         * @return Temps total de l'élément passé en paramètre
         */
        unsigned int setSongDetails(SongListItem *item) const;

        /**
         * @brief Ajoute l'élément passé en paramètre comme fils du parent.
         * @param item Elément à ajouter
         * @param parent Parent du nouvel élément
         * @param pos Position à laquelle ajouter l'élément
         */
        void addChildSong(SongListItem *item, SongListItem *parent, int pos = -1) const;

        /**
         * @brief Supprime de la liste l'élément passé en paramètre (avec ses parents récursivement s'il s'agit du seul fils).
         * @param it Itérateur sur l'élément à supprimer
         */
        void removeSong(const SongListIterator& it);

    private slots:

        void onItemClicked(QTreeWidgetItem *item, int column);

    protected:

        virtual void mouseMoveEvent(QMouseEvent *event) override;

        virtual void leaveEvent(QEvent *event) override;

    signals:

        /**
         * @brief Signal émis lorsque l'utilisateur clique sur une musique de la liste.
         * @param songId Musique sélectionnée
         */
        void songPressed(audio::Player::SongId songId);

        /**
         * @brief Signal émis lorsque l'utilisateur supprime sur une musique de la liste.
         * @param song Musique supprimée
         */
        void songRemoved(audio::Player::SongId song);

    public:

        SongList(QWidget *parent = nullptr);
        virtual ~SongList() = default;

        /**
         * @brief Vide la liste du noeud passé en paramètre.
         * @param list Liste à nettoyer (locale/distante)
         */
        void clearList(SongList_t list = SongList_t::LOCAL_SONGS);

        /**
         * @brief Change le son marqué comme son courant dans la liste.
         * @param songId Indice de la musique actuelle
        */
        void setCurrentSong(audio::Player::SongId songId);

        /**
         * @brief getSongHierarchy
         * @param list Liste à retourner
         * @return Arborescence de musiques de la liste.
         */
        SongTreeRoot* getSongHierarchy(SongList_t list = SongList_t::LOCAL_SONGS) const;

        /**
         * @brief Ajoute le son passé en paramètre dans la liste.
         * @param list Type de liste dans laquelle ajouter la musique
         * @param item Elément à ajouter
         */
        void addSong(SongList_t list, SongListItem *item);

        /**
         * @brief Ajoute les titres et durées de la liste passée en paramètre.
         * @param list Type de liste dans laquelle ajouter les musiques (locales, distantes)
         * @param songs Liste de sons à ajouter
        */
        void addTree(SongList_t list, SongTreeRoot *songs);

        /**
         * @brief Supprime de la liste l'élément passé en paramètre (avec ses parents récursivement s'il s'agit du seul fils).
         * @param item Elément à supprimer
         */
        void removeSong(SongListItem *item);

    public slots:

        /**
         * @brief Affiche la musique passée en paramètre comme indisponible.
         * @param songId Musique à désactiver
         */
        void disableSong(audio::Player::SongId songId);
};


} // gui

#endif  // __SONGLIST_H__
