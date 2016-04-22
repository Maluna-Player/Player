/*************************************
 * @file    CommandReply.cpp
 * @date    19/06/15
 * @author  Manuel
 *
 * Définitions de la classe CommandReply.
 *************************************
*/

#include "CommandReply.h"
#include <QDataStream>


namespace network { namespace commands {


CommandReply::CommandReply(audio::Player::SongId songNum, FMOD_RESULT result) : Command(songNum), m_Result(result)
{

}

FMOD_RESULT CommandReply::getResult() const
{
    return m_Result;
}

bool CommandReply::isRequest() const
{
    return false;
}

bool CommandReply::isReply() const
{
    return true;
}

QByteArray CommandReply::toPacket() const
{
    QByteArray packet = Command::toPacket();
    QDataStream out(&packet, QIODevice::Append);

    out << static_cast<quint8>(getResult());

    return packet;
}

// ==============================
// ==============================

OpenCommandReply::OpenCommandReply(audio::Player::SongId songNum, FMOD_RESULT result, int fileSize)
    : CommandReply(songNum, result), m_FileSize(fileSize)
{

}

char OpenCommandReply::getCommandType() const
{
    return 'o';
}

int OpenCommandReply::getFileSize() const
{
    return m_FileSize;
}

QByteArray OpenCommandReply::toPacket() const
{
    QByteArray packet = CommandReply::toPacket();
    QDataStream out(&packet, QIODevice::Append);

    out << static_cast<quint32>(getFileSize());

    return packet;
}

// ==============================
// ==============================

CloseCommandReply::CloseCommandReply(audio::Player::SongId songNum, FMOD_RESULT result)
    : CommandReply(songNum, result)
{

}

char CloseCommandReply::getCommandType() const
{
    return 'c';
}

// ==============================
// ==============================

ReadCommandReply::ReadCommandReply(audio::Player::SongId songNum, FMOD_RESULT result, void *buffer, unsigned int bytes)
    : CommandReply(songNum, result), m_Buffer(buffer), m_ReadBytes(bytes)
{

}

ReadCommandReply::~ReadCommandReply()
{
    delete m_Buffer;
}

char ReadCommandReply::getCommandType() const
{
    return 'r';
}

void* ReadCommandReply::getBuffer() const
{
    return m_Buffer;
}

unsigned int ReadCommandReply::getReadBytes() const
{
    return m_ReadBytes;
}

QByteArray ReadCommandReply::toPacket() const
{
    QByteArray packet = CommandReply::toPacket();
    QDataStream out(&packet, QIODevice::Append);

    out << static_cast<quint32>(getReadBytes());
    out.writeRawData(static_cast<char*>(getBuffer()), getReadBytes());

    return packet;
}

// ==============================
// ==============================

SeekCommandReply::SeekCommandReply(audio::Player::SongId songNum, FMOD_RESULT result)
    : CommandReply(songNum, result)
{

}

char SeekCommandReply::getCommandType() const
{
    return 's';
}


} // commands
} // network
