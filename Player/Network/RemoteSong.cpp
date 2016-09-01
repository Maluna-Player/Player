/*************************************
 * @file    RemoteSong.cpp
 * @date    16/06/15
 * @author  Manuel
 *
 * Définitions de la classe RemoteSong.
 *************************************
*/

#include "RemoteSong.h"
#include "PlayerSocket.h"


namespace network {


RemoteSong::RemoteSong(audio::Player::SongId num, const QString& file, audio::Player::SongId remoteNum, audio::SoundPos_t length, const QString& artist, audio::SoundSettings *settings)
    : Song(num, file, false, false), m_RemoteNum(remoteNum), m_Settings(settings)
{
    m_Length = length;
    m_File = QString::number(remoteNum);
    m_Artist = artist;
}

// ==============================
// ==============================

RemoteSong::~RemoteSong()
{

}

// ==============================
// ==============================

bool RemoteSong::isRemote() const
{
    return true;
}

// ==============================
// ==============================

audio::Player::SongId RemoteSong::getRemoteNum() const
{
    return m_RemoteNum;
}

// ==============================
// ==============================

void RemoteSong::open()
{
    m_SoundID = audio::FmodManager::getInstance().openFromFile(m_File.toStdString(), true, m_Settings);
}


} // network
