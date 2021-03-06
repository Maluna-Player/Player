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


namespace network {


class RemoteSong : public audio::Song
{
    private:

        audio::Player::SongId m_RemoteId;

        audio::SoundSettings *m_Settings;

    protected:

        friend class audio::Player;

        RemoteSong(audio::Player::SongId id, const QString& file, audio::Player::SongId remoteId, audio::SoundPos_t length, const QString& artist, audio::SoundSettings *settings);

    public:

        virtual ~RemoteSong() = default;

        virtual bool isRemote() const override;

        /**
         * @brief getRemoteId
         * @return Identifiant de la musique chez le pair.
         */
        audio::Player::SongId getRemoteId() const;

        /**
         * @brief Ouvre le fichier avec FMOD pour stream du fichier distant.
         */
        virtual void open() override;
};


} // network

#endif  // __REMOTESONG_H__

