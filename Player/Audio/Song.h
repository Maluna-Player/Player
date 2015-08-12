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

class Song
{
    private:

        int m_Num;
        QString m_Title;

    protected:

        QString m_File;

        SoundPos_t m_Length;

        SoundID_t m_SoundID;

    public:

        Song(const QString& file = "", int num = -1, bool openable = true);
        virtual ~Song();

        virtual SoundID_t getSoundID() const;
        virtual int getNum() const;
        virtual const QString& getFile() const;
        virtual SoundPos_t getLength() const;

        /**
         * @brief Change le numéro du son.
         * @param num Nouveau numéro
         */
        virtual void setNum(int num);

        /**
         * @brief Détermine si la musique est distante ou non.
         * @return true si musique distante
         */
        virtual bool isRemote() const;

        /**
         * @brief getTitle
         * @return Titre du son (tag) si le son en possède ou le nom du fichier.
         */
        virtual const QString& getTitle() const;

        /**
         * @brief Ouvre le fichier avec FMOD pour stream.
         */
        virtual void open();

        /**
         * @brief Joue le son ouvert avec FMOD.
         */
        virtual void play() const;

        /**
         * @brief Met le son en pause ou le redémarre.
         * @param paused Etat pause à mettre
         */
        virtual void pause(bool paused) const;

        /**
         * @brief Stoppe le son.
         */
        virtual void stop() const;

        /**
         * @brief getPosition
         * @return Position de la musique (ms)
         */
        virtual SoundPos_t getPosition() const;

        /**
         * @brief Modifie la position de la musique.
         * @param pos Nouvelle position en ms
         */
        virtual void setPosition(SoundPos_t pos) const;

        /**
         * @brief isFinished
         * @return true si la musique est terminée.
         */
        virtual bool isFinished() const;
};

#endif  // __SONG_H__
