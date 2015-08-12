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

class CommandReply : public Command
{
    private:

        FMOD_RESULT m_Result;

    public:

        CommandReply(int songNum, FMOD_RESULT result);
        virtual ~CommandReply() {}

        virtual FMOD_RESULT getResult() const;

        virtual bool isRequest() const;

        virtual bool isReply() const;

        virtual QByteArray toPacket() const;
};

class OpenCommandReply : public CommandReply
{
    private:

        int m_FileSize;

    public:

        OpenCommandReply(int songNum, FMOD_RESULT result, int fileSize);
        virtual ~OpenCommandReply() {}

        virtual char getCommandType() const;

        virtual int getFileSize() const;

        virtual QByteArray toPacket() const;
};

class CloseCommandReply : public CommandReply
{
    public:

        CloseCommandReply(int songNum, FMOD_RESULT result);
        virtual ~CloseCommandReply() {}

        virtual char getCommandType() const;
};

class ReadCommandReply : public CommandReply
{
    private:

        void *m_Buffer;

        unsigned int m_ReadBytes;

    public:

        ReadCommandReply(int songNum, FMOD_RESULT result, void *buffer, unsigned int bytes);
        virtual ~ReadCommandReply();

        virtual char getCommandType() const;

        virtual void* getBuffer() const;

        virtual unsigned int getReadBytes() const;

        virtual QByteArray toPacket() const;
};

class SeekCommandReply : public CommandReply
{
    public:

        SeekCommandReply(int songNum, FMOD_RESULT result);
        virtual ~SeekCommandReply() {}

        virtual char getCommandType() const;
};

#endif  // __COMMANDREPLY_H__

