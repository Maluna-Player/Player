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

        QPixmap m_BrokenIcon;


        /**
         * @brief Récupère la racine correspondant à la liste passée en paramètre.
         * @param list Liste dont on veut récupérer la racine
         * @return Noeud racine
         */
        virtual SongListItem* getRootNode(SongList_t list) const;

        /**
         * @brief Affiche le titre et le temps de l'élément passé en paramètre et de ses fils.
         * @param item Elément parent contenant les informations à afficher
         * @return Temps total de l'élément passé en paramètre
         */
        virtual unsigned int setSongDetails(SongListItem *item) const;

        /**
         * @brief Ajoute le décalage nécessaire pour le temps de l'élément passé en paramètre et pour ses fils.
         * @param item Elément parent dont on ajoute le décalage
         * @param depth Profondeur de l'élement dans l'arborescence
         */
        virtual void setDepth(SongListItem *item, int depth) const;

        /**
         * @brief Ajoute l'élément passé en paramètre comme fils du parent.
         * @param item Elément à ajouter
         * @param parent Parent du nouvel élément
         */
        virtual void addChildSong(SongListItem *item, SongListItem *parent) const;

        /**
         * @brief Supprime de la liste l'élément passé en paramètre (avec ses parents récursivement s'il s'agit du seul fils).
         * @param it Itérateur sur l'élément à supprimer
         */
        virtual void removeSong(const SongListIterator& it) const;

    protected:

        virtual void mousePressEvent(QMouseEvent *event) override;

    signals:

        /**
         * @brief Signal émis lorsque l'utilisateur clique sur une musique de la liste.
         * @param songId Musique sélectionnée
         */
        void songPressed(audio::Player::SongId songId);

    public:

        SongList(QWidget *parent = nullptr);
        virtual ~SongList();

        /**
         * @brief Vide la liste du noeud passé en paramètre.
         * @param list Liste à nettoyer (locale/distante)
         */
        virtual void clearList(SongList_t list = SongList_t::LOCAL_SONGS);

        /**
         * @brief Change le son marqué comme son courant dans la liste.
         * @param songId Indice de la musique actuelle
        */
        virtual void setCurrentSong(audio::Player::SongId songId);

        /**
         * @brief getSongHierarchy
         * @param list Liste à retourner
         * @return Arborescence de musiques de la liste.
         */
        virtual SongTreeRoot* getSongHierarchy(SongList_t list = SongList_t::LOCAL_SONGS) const;

        /**
         * @brief Ajoute le son passé en paramètre dans la liste.
         * @param list Type de liste dans laquelle ajouter la musique
         * @param item Elément à ajouter
         */
        virtual void addSong(SongList_t list, SongListItem *item);

        /**
         * @brief Ajoute les titres et durées de la liste passée en paramètre.
         * @param list Type de liste dans laquelle ajouter les musiques (locales, distantes)
         * @param songs Liste de sons à ajouter
        */
        virtual void addTree(SongList_t list, SongTreeRoot *songs);

    public slots:

        /**
         * @brief Affiche la musique passée en paramètre comme indisponible.
         * @param songId Musique à désactiver
         */
        virtual void disableSong(audio::Player::SongId songId);
};


} // gui

#endif  // __SONGLIST_H__
