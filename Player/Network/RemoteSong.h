/*************************************
 * @file    RemoteSong.h
 * @date    16/06/15
 * @author  Manuel
 *
 * Déclarations de la classe RemoteSong
 * contenant les informations d'une musique distante.
 *************************************
*/

#ifndef __REMOTESONG_H__
#define __REMOTESONG_H__

#include "../Audio/Song.h"

class RemoteSong : public Song
{
    private:

        int m_RemoteNum;

    public:

        RemoteSong(const QString& file, int num, int remoteNum, SoundPos_t length, const QString& artist);
        virtual ~RemoteSong();

        virtual bool isRemote() const;

        /**
         * @brief Ouvre le fichier avec FMOD pour stream du fichier distant.
         */
        virtual void open();
};

#endif  // __REMOTESONG_H__

