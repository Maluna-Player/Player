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
    : m_Num(num), m_File(file), m_SoundID(0), m_Artist("")
{
    if (openable)
    {
        SoundID_t id = FmodManager::getInstance()->openFromFile(m_File.toStdString(), false);
        m_Length = FmodManager::getInstance()->getSoundLength(id);

        QString title = QString::fromStdString(FmodManager::getInstance()->getSongTag(id, "TITLE"));

        if (title.isEmpty())
        {
            QFileInfo fileInfo(getFile());
            m_Title = fileInfo.completeBaseName();
        }
        else
            m_Title = title;

        m_Artist = QString::fromStdString(FmodManager::getInstance()->getSongTag(id, "ARTIST"));
        if (m_Artist.isEmpty())
            m_Artist = "Artiste inconnu";

        FmodManager::getInstance()->releaseSound(id);
    }
    else
        m_Title = m_File;
}

// ==============================
// ==============================

Song::~Song()
{
    FmodManager::getInstance()->releaseSound(m_SoundID);
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

void Song::setNum(int num)
{
    m_Num = num;
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

const QString& Song::getArtist() const
{
    return m_Artist;
}

// ==============================
// ==============================

QPixmap Song::buildPicture() const
{
    unsigned int pictureDataLength(0);
    char *pictureData = FmodManager::getInstance()->getSongPictureData(m_SoundID, &pictureDataLength);

    QPixmap pixmap;

    if (pictureData)
        pixmap.loadFromData(reinterpret_cast<uchar*>(pictureData), pictureDataLength);

    return pixmap;
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
    return !(getPosition() < getLength());
}

