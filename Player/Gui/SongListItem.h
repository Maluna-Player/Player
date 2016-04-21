/*************************************
 * @file    SongListItem.h
 * @date    19/09/15
 * @author  Manuel
 *
 * Déclarations de la classe SongListItem
 * définissant un élément de la liste de musiques.
 *************************************
*/

#ifndef __SONGLISTITEM__
#define __SONGLISTITEM__

#include <QTreeWidgetItem>
#include "../Network/Sendable.h"

class Song;
class SongListItem;

using SongTreeRoot = SongListItem;

class SongListItem : public QTreeWidgetItem, public Sendable
{
    public:

        enum class ElementType { ROOT, SONG, DIRECTORY };

    private:

        static unsigned int cpt;

        unsigned int m_Num;

        ElementType m_Type;

        Song *mp_AttachedSong;


        /**
         * @brief getParentNum
         * @return Numéro du parent de l'élément, 0 si pas de parent
         */
        virtual unsigned int getParentNum() const;

    public:

        SongListItem(ElementType type, SongListItem *parent = nullptr, const QString& name = "");
        virtual ~SongListItem();

        /**
         * @brief isRoot
         * @return true si l'élément est une racine.
         */
        virtual bool isRoot() const;

        /**
         * @brief isSong
         * @return true si l'élément est une musique.
         */
        virtual bool isSong() const;

        /**
         * @brief getNum
         * @return Numéro de l'élément.
         */
        virtual unsigned int getNum() const;

        /**
         * @brief parent
         * @return Parent de l'élément.
         */
        virtual SongListItem* parent() const;

        /**
         * @brief getAttachedSong
         * @return Son attaché à l'élément de l'arborescence.
         */
        virtual Song* getAttachedSong() const;

        /**
         * @brief Attache l'élément courant au parent passé en paramètre.
         * @param parent Nouveau parent de l'élément courant
         */
        virtual void setParent(SongListItem *parent);

        /**
         * @brief Attache le son passé en paramètre à l'élément courant.
         * @param song Son à attacher
         */
        virtual void setAttachedSong(Song *song);

        virtual QByteArray toPacket() const override;
};

#endif  // __SONGLISTITEM__

