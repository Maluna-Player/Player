/*************************************
 * @file    Player.h
 * @date    27/07/14
 * @author  Manuel
 *
 * Déclarations de la classe Player
 * contenant la gestion de l'ensemble
 * des musiques.
 *************************************
*/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>
#include <utility>
#include "Song.h"

#define FIRST_SONG        0
#define LAST_SONG         (static_cast<int>(m_Songs.size()) - 1)
#define UNDEFINED_SONG   -1

class Player
{
  private:

    std::vector<Song> m_Songs;
    int m_CurrentSong;

    bool m_Playlist;
    bool m_Loop;

    bool m_Pause;
    bool m_Stop;

    bool m_Mute;
    int m_VolumeState;

  public:

    Player();
    virtual ~Player();

    /**
     * @return Référence vers le son actuel.
    */
    virtual Song& getCurrentSong();

    /**
     * @return Liste des musiques (titre + durée) du dossier.
    */
    virtual std::vector<std::pair<std::string, int> > getSongDetails() const;

    /**
     * Active la lecture.
    */
    virtual void play();

    /**
     * Stoppe le player.
    */
    virtual void stop();

    /**
     * Met en pause le player.
    */
    virtual void pause();

    /**
     * Change l'état mute du player (mute/unmute).
     * @param mute Etat mute à appliquer
    */
    virtual void mute(bool mute);

    /**
     * @return true si ni pause, ni stop.
    */
    virtual bool isPlayed() const;

    /**
     * @return true si le player est stoppé.
    */
    virtual bool isStopped() const;

    /**
     * @return true si le player est en pause.
    */
    virtual bool isPaused() const;

    /**
     * @return true si le player est mute.
    */
    virtual bool isMuted() const;

    /**
     * @return true si la boucle est activée.
    */
    virtual bool isLoop() const;

    /**
     * @param loop Valeur à affecter à l'attribut Loop.
    */
    virtual void setLoop(bool loop);

    /**
     * @return numéro de la première chanson,
     *         UNDEFINED_SONG si pas de chanson.
    */
    virtual int first() const;

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
     * @return Etat du volume
    */
    virtual int getVolumeState() const;

    /**
     * Modifie le volume du player.
     * @param volumeState Etat du volume
    */
    virtual void setVolume(int volumeState);

    /**
     * Rempli le vecteur Musiques à partir des fichiers
     * contenus dans le répertoire donné en paramètre.
     * @param dir Répertoire à parcourir
    */
    virtual void loadSongs(const std::string& dir);

    /**
     * Lance la musique d'indice song.
     * @param song Indice de la musique
    */
    virtual void changeSong(int song);
};

#endif  // __PLAYER_H__
