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
    Q_OBJECT

    private:

        int m_CurrentSong;

        /**
         * @brief Détermine si l'item de la liste passé en paramètre est une musique ou non.
         * @param item Elément de la liste à vérifier
         * @return true si c'est une musique
         */
        virtual bool isSong(QTreeWidgetItem *item) const;

    protected:

        virtual void mousePressEvent(QMouseEvent *event);

    signals:

        /**
         * @brief Signal émis lorsque l'utilisateur clique sur une musique de la liste.
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
         * @return Liste des objets de l'arborescence.
         */
        virtual const QList<QTreeWidgetItem*> getSongHierarchy(SongList_t list = LOCAL_SONGS) const;

        /**
         * @brief Ajoute les titres et durées de la liste passée en paramètre.
         * @param list Type de liste dans laquelle ajouter les musiques (locales, distantes)
         * @param songs Liste de sons à ajouter
        */
        virtual void add(SongList_t list, const QList<QTreeWidgetItem*>& songs);
};

#endif  // __SONGLIST_H__
