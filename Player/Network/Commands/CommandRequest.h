/*************************************
 * @file    CommandRequest.h
 * @date    01/07/15
 * @author  Manuel
 *
 * Déclarations des classes définissant
 * les différentes requêtes émises par un client.
 *************************************
*/

#ifndef __COMMANDREQUEST_H__
#define __COMMANDREQUEST_H__

#include "Command.h"


namespace network { namespace commands {


class CommandRequest : public Command
{
    public:

        CommandRequest(audio::Player::SongId songId);
        virtual ~CommandRequest() {}

        virtual bool isRequest() const override;

        virtual bool isReply() const override;
};

class OpenCommandRequest : public CommandRequest
{
    public:

        OpenCommandRequest(audio::Player::SongId songId);
        virtual ~OpenCommandRequest() {}

        virtual char getCommandType() const override;
};

class CloseCommandRequest : public CommandRequest
{
    public:

        CloseCommandRequest(audio::Player::SongId songId);
        virtual ~CloseCommandRequest() {}

        virtual char getCommandType() const override;
};

class ReadCommandRequest : public CommandRequest
{
    private:

        unsigned int m_BytesToRead;

    public:

        ReadCommandRequest(audio::Player::SongId songId, unsigned int bytes);
        virtual ~ReadCommandRequest() {}

        virtual char getCommandType() const override;

        virtual unsigned int getBytesToRead() const;

        virtual QByteArray toPacket() const override;
};

class SeekCommandRequest : public CommandRequest
{
    private:

        unsigned int m_Pos;

    public:

        SeekCommandRequest(audio::Player::SongId songId, unsigned int pos);
        virtual ~SeekCommandRequest() {}

        virtual char getCommandType() const override;

        virtual unsigned int getPos() const;

        virtual QByteArray toPacket() const override;
};


} // commands
} // network

#endif  // __COMMANDREQUEST_H__

