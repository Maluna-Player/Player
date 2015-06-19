/*************************************
 * @file    Song.cpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Définitions de la classe Song.
 *************************************
*/

#include "Song.h"
#include <QFileInfo>


Song::Song(const QString& file, int num, bool openable)
    : m_File(file), m_Num(num), m_SoundID(0)
{
    if (openable)
    {
        SoundID_t id = FmodManager::getInstance()->openFromFile(m_File.toStdString());
        m_Length = FmodManager::getInstance()->getSoundLength(id);

        QString title = QString::fromStdString(FmodManager::getInstance()->getSongTitle(id));

        if (title.isEmpty())
        {
            QFileInfo fileInfo(getFile());
            m_Title = fileInfo.completeBaseName();
        }
        else
            m_Title = title;
    }
    else
        m_Title = m_File;
}

// ==============================
// ==============================

Song::~Song()
{

}

// ==============================
// ==============================

SoundID_t Song::getSoundID() const
{
    return m_SoundID;
}

// ==============================
// ==============================

int Song::getNum() const
{
    return m_Num;
}

// ==============================
// ==============================

const QString& Song::getFile() const
{
    return m_File;
}

// ==============================
// ==============================

SoundPos_t Song::getLength() const
{
    return m_Length;
}

// ==============================
// ==============================

bool Song::isRemote() const
{
    return false;
}

// ==============================
// ==============================

const QString& Song::getTitle() const
{
    return m_Title;
}

// ==============================
// ==============================

void Song::open()
{
    m_SoundID = FmodManager::getInstance()->openFromFile(m_File.toStdString());
}

// ==============================
// ==============================

void Song::play() const
{
    FmodManager::getInstance()->playSound(m_SoundID);
}

// ==============================
// ==============================

void Song::pause(bool paused) const
{
    FmodManager::getInstance()->pauseSound(m_SoundID, paused);
}

// ==============================
// ==============================

void Song::stop() const
{
    FmodManager::getInstance()->stopSound(m_SoundID);
}

// ==============================
// ==============================

SoundPos_t Song::getPosition() const
{
    return FmodManager::getInstance()->getSoundPosition(m_SoundID);
}

// ==============================
// ==============================

void Song::setPosition(SoundPos_t pos) const
{
    if (pos >= m_Length)
        pos = m_Length - 1;

    FmodManager::getInstance()->setSoundPosition(m_SoundID, pos);
}

// ==============================
// ==============================

bool Song::isFinished() const
{
    return !(FmodManager::getInstance()->isPlaying(m_SoundID));
}

