/*************************************
 * @file    Song.hpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Déclarations de la classe Song
 * contenant les informations d'une musique.
 *************************************
*/

#ifndef __SONG_HPP__
#define __SONG_HPP__

#include <string>
#include "Fmod.hpp"

class Song
{
  private:

    std::string m_File;
    int m_Num;
    SoundPos_t m_Length;

    SoundID_t m_SoundID;
    int m_VolumeState;

  public:

		Song(const std::string& file, int num);
		Song(const Song& song);
		virtual ~Song();

    virtual SoundID_t getSoundID() const;
    virtual std::string getFile() const;
    virtual SoundPos_t getLength() const;

    /**
     * Joue le son avec FMOD.
    */
    virtual void play();

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
     * @return Position de la musique.
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

    /**
     * @return Etat du volume
    */
    virtual int getVolumeState() const;

    /**
     * Modifie le volume de la musique.
     * @param volumeState Etat du volume
    */
    virtual void setVolume(int volumeState);
};

#endif  // __SONG_HPP__
