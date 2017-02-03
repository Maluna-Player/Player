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


RemoteSong::RemoteSong(audio::Player::SongId id, const QString& file, audio::Player::SongId remoteId, audio::SoundPos_t length, const QString& artist, audio::SoundSettings *settings)
    : Song(id, file, false, false), m_RemoteId(remoteId), m_Settings(settings)
{
    m_Length = length;
    m_File = QString::number(remoteId);
    m_Artist = artist;
}

// ==============================
// ==============================

bool RemoteSong::isRemote() const
{
    return true;
}

// ==============================
// ==============================

audio::Player::SongId RemoteSong::getRemoteId() const
{
    return m_RemoteId;
}

// ==============================
// ==============================

void RemoteSong::open()
{
    m_SoundID = audio::FmodManager::getInstance().openFromFile(m_File.toStdString(), true, m_Settings);
}


} // network
