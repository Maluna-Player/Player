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


Command::Command(Player::SongId songNum) : m_SongNum(songNum)
{

}

// ==============================
// ==============================

Player::SongId Command::getSongNum() const
{
    return m_SongNum;
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
    out << static_cast<quint16>(getSongNum());

    return packet;
}
