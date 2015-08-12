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

class Command : public Sendable
{
    private:

        int m_SongNum;

    public:

        Command(int songNum);
        virtual ~Command() {}

        virtual int getSongNum() const;

        virtual bool isRequest() const = 0;

        virtual bool isReply() const = 0;

        virtual char getCommandType() const = 0;

        virtual QByteArray toPacket() const;
};

#endif  // __COMMAND_H__

