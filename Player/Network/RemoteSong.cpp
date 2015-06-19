/*************************************
 * @file    RemoteSong.cpp
 * @date    16/06/15
 * @author  Manuel
 *
 * Définitions de la classe RemoteSong.
 *************************************
*/

#include "RemoteSong.h"


RemoteSong::RemoteSong(const QString& file, int num, SoundPos_t length)
    : Song(file, num, false)
{
    m_Length = length;
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
