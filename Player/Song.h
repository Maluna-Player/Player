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

#include <string>
#include "FmodManager.h"

class Song
{
  private:

    std::string m_File;
    int m_Num;
    SoundPos_t m_Length;
    std::string m_Title;

    SoundID_t m_SoundID;

  public:

		Song(const std::string& file, int num);
		virtual ~Song();

    virtual SoundID_t getSoundID() const;
    virtual int getNum() const;
    virtual std::string getFile() const;
    virtual SoundPos_t getLength() const;

    /**
     * @return Titre du son (tag) si le son en possède
     *         ou le nom du fichier.
    */
    virtual std::string getTitle() const;

    /**
     * Ouvre le fichier avec FMOD pour stream.
    */
    virtual void open();

    /**
     * Joue le son ouvert avec FMOD.
    */
    virtual void play() const;

    /**
     * Met le son en pause ou le redémarre.
     * @param paused Etat pause à mettre
    */
    virtual void pause(bool paused) const;

    /**
     * Stoppe le son.
    */
    virtual void stop() const;

    /**
     * @return Position de la musique (ms).
    */
    virtual SoundPos_t getPosition() const;

    /**
     * Modifie la position de la musique.
     * @param pos Nouvelle position en ms
    */
    virtual void setPosition(SoundPos_t pos) const;

    /**
     * @return true si la musique est terminée.
    */
    virtual bool isFinished() const;

};

#endif  // __SONG_H__
