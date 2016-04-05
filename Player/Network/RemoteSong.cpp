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


RemoteSong::RemoteSong(const QString& file, int num, int remoteNum, SoundPos_t length, const QString& artist, SoundSettings *settings)
    : Song(file, num, false, false), m_RemoteNum(remoteNum), m_Settings(settings)
{
    m_Length = length;
    m_File = QString::number(num);
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

void RemoteSong::open()
{
    m_SoundID = FmodManager::getInstance()->openFromFile(m_File.toStdString(), true, m_Settings);
}
