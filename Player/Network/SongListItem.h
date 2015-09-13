/*************************************
 * @file    SongListItem.h
 * @date    02/07/15
 * @author  Manuel
 *
 * Déclarations de la classe SongListItem
 * contenant les informations d'un élement
 * de l'arborescence à envoyer.
 *************************************
*/

#ifndef __SONGLISTITEM_H__
#define __SONGLISTITEM_H__

#include "Sendable.h"
#include <QString>

class SongListItem : public Sendable
{
    private:

        int m_Num;
        int m_Parent;
        QString m_Title;
        QString m_Artist;

        bool m_File;
        int m_Length;

    public:

        /**
         * @brief Constructeur de dossiers de l'arborescence.
         */
        SongListItem(int num, const QString& title, int parent = 0);

        /**
         * @brief Constructeur de musiques de l'arborescence.
         */
        SongListItem(int num, int length, const QString& title, const QString& artist, int parent = 0);

        virtual ~SongListItem();

        virtual QByteArray toPacket() const;
};

#endif  // __SONGLISTITEM_H__

