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

        Player::SongId m_RemoteNum;

        SoundSettings *m_Settings;

    protected:

        friend class Player;

        RemoteSong(Player::SongId num, const QString& file, Player::SongId remoteNum, SoundPos_t length, const QString& artist, SoundSettings *settings);

    public:

        virtual ~RemoteSong();

        virtual bool isRemote() const;

        /**
         * @brief getRemoteNum
         * @return Identifiant de la musique chez le pair.
         */
        virtual Player::SongId getRemoteNum() const;

        /**
         * @brief Ouvre le fichier avec FMOD pour stream du fichier distant.
         */
        virtual void open();
};

#endif  // __REMOTESONG_H__

