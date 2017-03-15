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
#include <taglib/fileref.h>


namespace audio {


Song::Song(Player::SongId id, const QString& file, bool inFolder, bool openable)
    : m_Id(id), m_InFolder(inFolder), m_Available(true), m_File(file), m_SoundID(0), m_Artist("")
{
    if (openable)
    {
        SoundID_t fmodId = FmodManager::getInstance().openFromFile(m_File.toStdString(), false);
        m_Length = FmodManager::getInstance().getSoundLength(fmodId);

        readTags();

        FmodManager::getInstance().releaseSound(fmodId);
    }
    else
        m_Title = m_File;
}

// ==============================
// ==============================

SoundID_t Song::getSoundID() const
{
    return m_SoundID;
}

// ==============================
// ==============================

Player::SongId Song::getId() const
{
    return m_Id;
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

bool Song::isInFolder() const
{
    return m_InFolder;
}

// ==============================
// ==============================

bool Song::isRemote() const
{
    return false;
}

// ==============================
// ==============================

bool Song::isAvailable() const
{
    return m_Available;
}

// ==============================
// ==============================

void Song::setAvailable(bool value)
{
    m_Available = value;
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

void Song::readTags()
{
    TagLib::FileRef file(m_File.toStdString().c_str());

    if (!file.isNull() && file.tag())
    {
        TagLib::Tag *tag = file.tag();

        m_Title = QString::fromStdWString(tag->title().toWString());
        m_Artist = QString::fromStdWString(tag->artist().toWString());
    }

    if (m_Title.isEmpty())
    {
        QFileInfo fileInfo(getFile());
        m_Title = fileInfo.completeBaseName();
    }

    if (m_Artist.isEmpty())
        m_Artist = "Artiste inconnu";
}

// ==============================
// ==============================

QPixmap Song::buildPicture() const
{
    unsigned int pictureDataLength(0);
    char *pictureData = FmodManager::getInstance().getSongPictureData(m_SoundID, &pictureDataLength);

    QPixmap pixmap;

    if (pictureData)
        pixmap.loadFromData(reinterpret_cast<uchar*>(pictureData), pictureDataLength);

    return pixmap;
}

// ==============================
// ==============================

void Song::open()
{
    m_SoundID = FmodManager::getInstance().openFromFile(m_File.toStdString());
}

// ==============================
// ==============================

void Song::play() const
{
    FmodManager::getInstance().playSound(m_SoundID);
}

// ==============================
// ==============================

void Song::pause(bool paused) const
{
    FmodManager::getInstance().pauseSound(m_SoundID, paused);
}

// ==============================
// ==============================

void Song::stop() const
{
    FmodManager::getInstance().stopSound(m_SoundID);
}

// ==============================
// ==============================

SoundPos_t Song::getPosition() const
{
    return FmodManager::getInstance().getSoundPosition(m_SoundID);
}

// ==============================
// ==============================

void Song::setPosition(SoundPos_t pos) const
{
    if (pos >= m_Length)
        pos = m_Length - 1;

    FmodManager::getInstance().setSoundPosition(m_SoundID, pos);
}

// ==============================
// ==============================

bool Song::isFinished() const
{
    return !(getPosition() < getLength());
}


} // audio
