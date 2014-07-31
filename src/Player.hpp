/*************************************
 * @file    Player.hpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Déclarations de la classe Player
 * contenant la gestion de l'ensemble
 * des musiques.
 *************************************
*/

#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <vector>
#include "Song.hpp"

class Player
{
  private:

    std::vector<Song> m_Songs;
    int m_CurrentSong;

    bool m_Playlist;
    bool m_Loop;

    bool m_Stop;

  public:

    Player();
    virtual ~Player();

    virtual const Song& getCurrentSong() const;

    /**
     * Rempli le vecteur Musiques à partir des fichiers
     * contenus dans le répertoire donné en paramètre.
     * @param dir Répertoire à parcourir
     * @return true si tout s'est bien passé
    */
    virtual bool loadSongs(const std::string& dir);

    /**
     * Lance l'ensemble des musiques chargées
     * à partir du répertoire.
    */
    virtual void playAllSongs(bool loop = false);

    /**
     * Lance la prochaine musique de la playlist.
    */
    virtual void nextSong();

    /**
     * @return true si le player est stoppé.
    */
    virtual bool isStopped() const;
};

#endif  // __PLAYER_HPP__
