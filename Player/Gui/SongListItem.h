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
#include <memory>
#include "../Network/Sendable.h"


namespace audio
{
    class Song;
}


namespace gui {


class SongListItem;
using SongTreeRoot = SongListItem;

class SongListItem : public QTreeWidgetItem, public network::Sendable
{
    public:

        enum class ElementType { ROOT, SONG, DIRECTORY };

    private:

        static unsigned int m_Cpt;

        unsigned int m_Num;

        ElementType m_Type;

        unsigned int m_Length;

        unsigned int m_Depth;

        std::shared_ptr<audio::Song> mp_AttachedSong;


        /**
         * @brief getParentNum
         * @return Numéro du parent de l'élément, 0 si pas de parent
         */
        virtual unsigned int getParentNum() const;

        /**
         * @brief Modifie le texte de la durée avec la valeur et profondeur courantes.
         */
        virtual void setLengthText();

    public:

        SongListItem(ElementType type, const QString& name = "", SongListItem *parent = nullptr);
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
        virtual std::shared_ptr<audio::Song> getAttachedSong() const;

        /**
         * @brief Attache l'élément courant au parent passé en paramètre.
         * @param parent Nouveau parent de l'élément courant
         */
        virtual void setParent(SongListItem *parent);

        /**
         * @brief Attache le son passé en paramètre à l'élément courant.
         * @param song Son à attacher
         */
        virtual void setAttachedSong(std::shared_ptr<audio::Song> song);

        /**
         * @brief getLength
         * @return Temps de l'élément.
         */
        virtual unsigned int getLength() const;

        /**
         * @brief Modifie le temps de l'élément.
         * @param length Nouveau temps
         */
        virtual void setLength(unsigned int length);

        /**
         * @brief getDepth
         * @return Profondeur de l'élément.
         */
        virtual unsigned int getDepth() const;

        /**
         * @brief Modifie la profondeur de l'élément.
         * @param depth Nouvelle profondeur
         */
        virtual void setDepth(unsigned int depth);

        /**
         * @brief Change la couleur du texte de l'élément.
         * @param color Nouvelle couleur
         */
        virtual void setTextColor(const QColor& color);

        virtual QByteArray toPacket() const override;
};


} // gui

#endif  // __SONGLISTITEM__

