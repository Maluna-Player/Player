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
    public:

        RemoteSong(const QString& file, int num, SoundPos_t length);
        virtual ~RemoteSong();

        virtual bool isRemote() const;
};

#endif  // __REMOTESONG_H__

