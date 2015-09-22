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

#include <QVector>
#include "../Gui/SongListItem.h"
#include <QFile>
#include "Song.h"
#include "../Network/PlayerSocket.h"
#include "../Network/Commands/Command.h"
#include "../Network/Commands/CommandRequest.h"
#include "../Network/Commands/CommandReply.h"

#define FIRST_SONG        0
#define LAST_SONG         (static_cast<int>(mp_Songs.size()) - 1)
#define UNDEFINED_SONG   -1

class Player : public QObject
{
    Q_OBJECT

    private:

        QVector<Song*> mp_Songs;
        int m_CurrentSong;

        bool m_Playlist;
        bool m_Loop;

        bool m_Pause;
        bool m_Stop;

        bool m_Mute;
        int m_VolumeState;

        QFile clientFile;

    signals:

        /**
         * @brief Signal émis lorsque la commande reçue a été traitée et que la réponse est créée.
         */
        void commandExecuted(CommandReply*);

    public:

        Player();
        virtual ~Player();

        /**
         * @brief getCurrentSong
         * @return Référence vers le son actuel.
         */
        virtual Song& getCurrentSong();

        /**
         * @brief countSongs
         * @return Nombre de musiques enregistrées.
         */
        virtual int songsCount() const;

        /**
         * @brief Active la lecture.
         */
        virtual void play();

        /**
         * @brief Stoppe le player.
         */
        virtual void stop();

        /**
         * @brief Met en pause le player.
         */
        virtual void pause();

        /**
         * @brief Change l'état mute du player (mute/unmute).
         * @param mute Etat mute à appliquer
         */
        virtual void mute(bool mute);

        /**
         * @brief isPlaying
         * @return true si ni pause, ni stop.
         */
        virtual bool isPlaying() const;

        /**
         * @brief isStopped
         * @return true si le player est stoppé.
         */
        virtual bool isStopped() const;

        /**
         * @brief isPaused
         * @return true si le player est en pause.
         */
        virtual bool isPaused() const;

        /**
         * @brief isMuted
         * @return true si le player est mute.
         */
        virtual bool isMuted() const;

        /**
         * @brief isLoop
         * @return true si la boucle est activée.
         */
        virtual bool isLoop() const;

        /**
         * @brief Applique ou retire l'état loop au player.
         * @param loop Valeur à affecter à l'attribut Loop.
         */
        virtual void setLoop(bool loop);

        /**
         * @brief first
         * @return numéro de la première chanson,
         *         UNDEFINED_SONG si pas de chanson.
         */
        virtual int first() const;

        /**
         * @brief prev
         * @return numéro de chanson précédente,
         *         UNDEFINED_SONG si pas de précédent.
         */
        virtual int prev() const;

        /**
         * @brief next
         * @return numéro de la chanson suivante,
         *         UNDEFINED_SONG si pas de suivant.
         */
        virtual int next() const;

        /**
         * @brief getVolumeState
         * @return Etat du volume.
         */
        virtual int getVolumeState() const;

        /**
         * @brief Modifie le volume du player.
         * @param volumeState Etat du volume
         */
        virtual void setVolume(int volumeState);

        /**
         * @brief Vide la liste des musiques du player.
         */
        virtual void clearSongs();

        /**
         * @brief Ajoute une nouvelle musique dans la liste du player.
         * @param filePath Chemin du fichier à ajouter
         * @param parentDir Parent dans l'arborescence
         */
        virtual void addNewSong(const QString& filePath, SongListItem *parentDir = 0);

        /**
         * @brief Remplit le vecteur Musiques à partir des fichiers
         *        contenus dans le répertoire donné en paramètre.
         * @param dirPath Répertoire à parcourir
         * @param parentDir Parent dans l'arborescence
         * @return Arborescence des fichiers lus
         */
        virtual SongTreeRoot* loadSongs(const QString& dirPath, SongTreeRoot *parentDir = 0);

        /**
         * @brief Ajoute les sons de l'arborescence passée en paramètre à la liste du player.
         * @param songs Arborescence de musiques à ajouter
         */
        virtual void addSongs(SongTreeRoot *songs);

        /**
         * @brief Supprime les musiques distantes de la liste.
         */
        virtual void removeRemoteSongs();

        /**
         * @brief Lance la musique d'indice song.
         * @param song Indice de la musique
         */
        virtual void changeSong(int song);

    public slots:

        /**
         * @brief Traite la commande passée en paramètre et émet la réponse une fois terminée.
         * @param command Commande à traiter
         */
        virtual void executeNetworkCommand(CommandRequest *command);
};

#endif  // __PLAYER_H__
