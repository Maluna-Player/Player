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

#define FIRST_SONG        0
#define LAST_SONG         (m_Songs.size() - 1)
#define UNDEFINED_SONG   -1

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

    virtual bool isLoop() const;

    virtual void setLoop(bool loop);

    /**
     * @return numéro de chanson précédente,
     *         UNDEFINED_SONG si pas de précédent.
    */
    virtual int prev() const;

    /**
     * @return numéro de la chanson suivante,
     *         UNDEFINED_SONG si pas de suivant.
    */
    virtual int next() const;

    /**
     * Rempli le vecteur Musiques à partir des fichiers
     * contenus dans le répertoire donné en paramètre.
     * @param dir Répertoire à parcourir
     * @return true si tout s'est bien passé
    */
    virtual bool loadSongs(const std::string& dir);

    /**
     * Lance la musique d'indice song.
     * @param song indice de la musique
    */
    virtual void playSong(int song);

    /**
     * @return true si le player est stoppé.
    */
    virtual bool isStopped() const;
};

#endif  // __PLAYER_HPP__
