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

class Command : public Sendable
{
    private:

        Player::SongId m_SongNum;

    public:

        Command(Player::SongId songNum);
        virtual ~Command() {}

        virtual Player::SongId getSongNum() const;

        virtual bool isRequest() const = 0;

        virtual bool isReply() const = 0;

        virtual char getCommandType() const = 0;

        virtual QByteArray toPacket() const override;
};

#endif  // __COMMAND_H__

