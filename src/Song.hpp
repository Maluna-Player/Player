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

class Song
{
  private:

    std::string m_File;
    int m_Num;
    unsigned int m_Length;

  public:

		Song(const std::string& file, int num);
		Song(const Song& song);
		virtual ~Song();

    virtual std::string getFile() const;
    virtual unsigned int getLength() const;
};

#endif  // __SONG_HPP__
