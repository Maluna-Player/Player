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


namespace network { namespace commands {


CommandRequest::CommandRequest(audio::Player::SongId songId) : Command(songId)
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

OpenCommandRequest::OpenCommandRequest(audio::Player::SongId songId)
    : CommandRequest(songId)
{

}

char OpenCommandRequest::getCommandType() const
{
    return 'o';
}

// ==============================
// ==============================

CloseCommandRequest::CloseCommandRequest(audio::Player::SongId songId)
    : CommandRequest(songId)
{

}

char CloseCommandRequest::getCommandType() const
{
    return 'c';
}

// ==============================
// ==============================

ReadCommandRequest::ReadCommandRequest(audio::Player::SongId songId, unsigned int bytes)
    : CommandRequest(songId), m_BytesToRead(bytes)
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

SeekCommandRequest::SeekCommandRequest(audio::Player::SongId songId, unsigned int pos)
    : CommandRequest(songId), m_Pos(pos)
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


} // commands
} // network
