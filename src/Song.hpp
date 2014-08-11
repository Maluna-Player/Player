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
    unsigned int m_Length;

    SoundID_t m_SoundID;

  public:

		Song(const std::string& file, int num);
		Song(const Song& song);
		virtual ~Song();

    virtual SoundID_t getSoundID() const;
    virtual std::string getFile() const;
    virtual unsigned int getLength() const;

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
     * @return true si la musique est terminée.
    */
    virtual bool isFinished() const;
};

#endif  // __SONG_HPP__
