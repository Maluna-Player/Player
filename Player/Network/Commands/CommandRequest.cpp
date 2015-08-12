/*************************************
 * @file    CommandRequest.cpp
 * @date    01/07/15
 * @author  Manuel
 *
 * Définitions de la classe CommandRequest.
 *************************************
*/

#include "CommandRequest.h"
#include <QDataStream>


CommandRequest::CommandRequest(int songNum) : Command(songNum)
{

}

bool CommandRequest::isRequest() const
{
    return true;
}

bool CommandRequest::isReply() const
{
    return false;
}

// ==============================
// ==============================

OpenCommandRequest::OpenCommandRequest(int songNum)
    : CommandRequest(songNum)
{

}

char OpenCommandRequest::getCommandType() const
{
    return 'o';
}

// ==============================
// ==============================

CloseCommandRequest::CloseCommandRequest(int songNum)
    : CommandRequest(songNum)
{

}

char CloseCommandRequest::getCommandType() const
{
    return 'c';
}

// ==============================
// ==============================

ReadCommandRequest::ReadCommandRequest(int songNum, unsigned int bytes)
    : CommandRequest(songNum), m_BytesToRead(bytes)
{

}

char ReadCommandRequest::getCommandType() const
{
    return 'r';
}

unsigned int ReadCommandRequest::getBytesToRead() const
{
    return m_BytesToRead;
}

QByteArray ReadCommandRequest::toPacket() const
{
    QByteArray packet = Command::toPacket();
    QDataStream out(&packet, QIODevice::Append);

    out << static_cast<quint32>(getBytesToRead());

    return packet;
}

// ==============================
// ==============================

SeekCommandRequest::SeekCommandRequest(int songNum, unsigned int pos)
    : CommandRequest(songNum), m_Pos(pos)
{

}

char SeekCommandRequest::getCommandType() const
{
    return 's';
}

unsigned int SeekCommandRequest::getPos() const
{
    return m_Pos;
}

QByteArray SeekCommandRequest::toPacket() const
{
    QByteArray packet = Command::toPacket();
    QDataStream out(&packet, QIODevice::Append);

    out << static_cast<quint32>(getPos());

    return packet;
}

