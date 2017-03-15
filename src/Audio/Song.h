/*************************************
 * @file    Song.h
 * @date    27/07/14
 * @author  Manuel
 *
 * Déclarations de la classe Song
 * contenant les informations d'une musique.
 *************************************
*/

#ifndef __SONG_H__
#define __SONG_H__

#include <QString>
#include "FmodManager.h"
#include "Player.h"
#include <QPixmap>


namespace audio {


class Song
{
    private:

        Player::SongId m_Id;

        QString m_Title;

        bool m_InFolder;

        bool m_Available;


        /**
         * @brief Modifie la disponibilité de la musique.
         * @param value Nouvelle disponibilité
         */
        void setAvailable(bool value);

        /**
         * @brief Remplit les champs de la musique à partir des tags.
         */
        void readTags();

    protected:

        friend class Player;

        QString m_File;

        SoundPos_t m_Length;

        SoundID_t m_SoundID;

        QString m_Artist;

        Song(Player::SongId id, const QString& file = "", bool inFolder = true, bool openable = true);

    public:

        virtual ~Song() = default;

        SoundID_t getSoundID() const;
        Player::SongId getId() const;
        const QString& getFile() const;
        SoundPos_t getLength() const;

        /**
         * @brief Détermine si la musique est dans le dossier de lecture ou non.
         * @return true si musique du dossier
         */
        bool isInFolder() const;

        /**
         * @brief Détermine si la musique est distante ou non.
         * @return true si musique distante
         */
        virtual bool isRemote() const;

        /**
         * @brief Détermine si la musique est accessible.
         * @return true si musique accessible
         */
        bool isAvailable() const;

        /**
         * @brief getTitle
         * @return Titre du son (tag) si le son en possède ou le nom du fichier.
         */
        const QString& getTitle() const;

        /**
         * @brief getArtist
         * @return Artiste du son (tag) si le son en possède, sinon "Artiste inconnu".
         */
        const QString& getArtist() const;

        /**
         * @brief Construit l'image de la pochette du son depuis le fichier.
         * @return Image associée au fichier, image vide sinon
         */
        QPixmap buildPicture() const;

        /**
         * @brief Ouvre le fichier avec FMOD pour stream.
         */
        virtual void open();

        /**
         * @brief Joue le son ouvert avec FMOD.
         */
        void play() const;

        /**
         * @brief Met le son en pause ou le redémarre.
         * @param paused Etat pause à mettre
         */
        void pause(bool paused) const;

        /**
         * @brief Stoppe le son.
         */
        void stop() const;

        /**
         * @brief getPosition
         * @return Position de la musique (ms)
         */
        SoundPos_t getPosition() const;

        /**
         * @brief Modifie la position de la musique.
         * @param pos Nouvelle position en ms
         */
        void setPosition(SoundPos_t pos) const;

        /**
         * @brief isFinished
         * @return true si la musique est terminée.
         */
        bool isFinished() const;
};


} // audio

#endif  // __SONG_H__
