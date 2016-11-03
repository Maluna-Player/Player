/*************************************
 * @file    Command.h
 * @date    19/06/15
 * @author  Manuel
 *
 * Déclarations des classes définissant
 * les différentes commandes échangées.
 *************************************
*/

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "../Sendable.h"
#include "../../Audio/Player.h"


namespace network { namespace commands {


class Command : public Sendable
{
    private:

        audio::Player::SongId m_SongId;

    public:

        Command(audio::Player::SongId songId);
        virtual ~Command() = default;

        audio::Player::SongId getSongId() const;

        virtual bool isRequest() const = 0;

        virtual bool isReply() const = 0;

        virtual char getCommandType() const = 0;

        virtual QByteArray toPacket() const override;
};


} // commands
} // network

#endif  // __COMMAND_H__

