/*************************************
 * @file    Command.cpp
 * @date    19/06/15
 * @author  Manuel
 *
 * Définitions de la classe Command.
 *************************************
*/

#include "Command.h"
#include <QDataStream>


namespace network { namespace commands {


Command::Command(audio::Player::SongId songId) : m_SongId(songId)
{

}

// ==============================
// ==============================

audio::Player::SongId Command::getSongId() const
{
    return m_SongId;
}

// ==============================
// ==============================

QByteArray Command::toPacket() const
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    char command = (isRequest()) ? 'C' : 'R' ;

    out << static_cast<quint8>(command);    // Commande

    out << static_cast<quint8>(getCommandType());
    out << static_cast<quint16>(getSongId());

    return packet;
}


} // commands
} // network
