/*************************************
 * @file    CommandReply.h
 * @date    19/06/15
 * @author  Manuel
 *
 * Déclarations de la classe CommandReply définissant
 * les réponses des différentes requêtes envoyées par un pair.
 *************************************
*/

#ifndef __COMMANDREPLY_H__
#define __COMMANDREPLY_H__

#include "Command.h"
#include <fmod.h>


namespace network { namespace commands {


class CommandReply : public Command
{
    private:

        FMOD_RESULT m_Result;

    public:

        CommandReply(audio::Player::SongId songId, FMOD_RESULT result);
        virtual ~CommandReply() = default;

        FMOD_RESULT getResult() const;

        virtual bool isRequest() const override;

        virtual bool isReply() const override;

        virtual QByteArray toPacket() const override;
};

class OpenCommandReply : public CommandReply
{
    private:

        unsigned int m_FileSize;

    public:

        OpenCommandReply(audio::Player::SongId songId, FMOD_RESULT result, unsigned int fileSize);
        virtual ~OpenCommandReply() = default;

        virtual char getCommandType() const override;

        unsigned int getFileSize() const;

        virtual QByteArray toPacket() const override;
};

class CloseCommandReply : public CommandReply
{
    public:

        CloseCommandReply(audio::Player::SongId songId, FMOD_RESULT result);
        virtual ~CloseCommandReply() = default;

        virtual char getCommandType() const override;
};

class ReadCommandReply : public CommandReply
{
    private:

        char *m_Buffer;

        unsigned int m_ReadBytes;

    public:

        ReadCommandReply(audio::Player::SongId songId, FMOD_RESULT result, char *buffer, unsigned int bytes);
        virtual ~ReadCommandReply();

        virtual char getCommandType() const override;

        char* getBuffer() const;

        unsigned int getReadBytes() const;

        virtual QByteArray toPacket() const override;
};

class SeekCommandReply : public CommandReply
{
    public:

        SeekCommandReply(audio::Player::SongId songId, FMOD_RESULT result);
        virtual ~SeekCommandReply() = default;

        virtual char getCommandType() const override;
};


} // commands
} // network

#endif  // __COMMANDREPLY_H__

