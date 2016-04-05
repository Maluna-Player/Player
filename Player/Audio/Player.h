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

#include "../Util/composedmap.h"
#include "../Constants.h"
#include "../Gui/SongListItem.h"
#include <QFile>
#include "Song.h"
#include "../Network/PlayerSocket.h"
#include "../Network/Commands/Command.h"
#include "../Network/Commands/CommandRequest.h"
#include "../Network/Commands/CommandReply.h"

#define FIRST_SONG      mp_Songs.begin()
#define UNDEFINED_SONG  mp_Songs.end()

class Player : public QObject
{
    Q_OBJECT

    private:

        using SongIt = ComposedMap<Constants::SongList_t, std::map<int, Song*>>::const_iterator;

        ComposedMap<Constants::SongList_t, std::map<int, Song*>> mp_Songs;
        SongIt m_CurrentSong;

        bool m_Playlist;
        bool m_Loop;

        bool m_Pause;
        bool m_Stop;

        bool m_Mute;
        int m_VolumeState;

        QFile clientFile;


        /**
         * @brief Détermine si le fichier passé en paramètre est déjà dans la liste.
         * @param filePath Chemin du fichier à tester
         * @return true si le fichier est déjà enregistré
         */
        virtual bool containsSong(const QString& filePath) const;

        /**
         * @brief Recherche une musique à partir de son identifiant.
         * @param song Identifiant de la musique
         * @return Itérateur sur le son correspondant au numéro passé en paramètre (end si pas trouvé)
         */
        virtual SongIt findSong(int song) const;

        /**
         * @brief Retourne la première musique de la liste.
         * @return Position de la première musique
         */
        virtual SongIt first() const;

        /**
         * @brief Retourne la musique précédente (la dernière si boucle).
         * @return Position de la musique précédente
         */
        virtual SongIt prev() const;

        /**
         * @brief Retourne la musique suivante (la première si boucle).
         * @return Position de la musique suivante
         */
        virtual SongIt next() const;

        /**
         * @brief Lance la musique passée en paramètre.
         * @param song Itérateur sur la musique
         * @return true si la musique a bien été modifiée
         */
        virtual bool changeSong(SongIt song);

    signals:

        /**
         * @brief Signal émis lorsque le son courant change.
         */
        void songChanged();

        /**
         * @brief Signal émis lorsque le player est stoppé.
         */
        void stopped();

        /**
         * @brief Signal émis lorsqu'une musique n'a pas pu être ouverte.
         */
        void streamError(QString);

        /**
         * @brief Signal émis lorsque la commande reçue a été traitée et que la réponse est créée.
         */
        void commandExecuted(CommandReply*);

    public:

        Player();
        virtual ~Player();

        /**
         * @brief getCurrentSong
         * @return Pointeur vers le son actuel.
         */
        virtual Song* getCurrentSong();

        /**
         * @brief Compte le nombre de musiques de la liste passée en paramètre.
         * @param list Liste dont on veut le nombre d'éléments
         * @return Nombre de musiques enregistrées.
         */
        virtual int songsCount(Constants::SongList_t list = Constants::ALL_SONGS) const;

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
         * @brief Vide la liste des musiques du player à partir de l'indice de la listte passé en paramètre.
         * @param list Liste dont on veut supprimer les éléments
         */
        virtual void clearSongs(Constants::SongList_t list = Constants::ALL_SONGS);

        /**
         * @brief Ajoute une nouvelle musique dans la liste du player.
         * @param list Liste à laquelle on veut ajouter la musique
         * @param filePath Chemin du fichier à ajouter
         * @param parentDir Parent dans l'arborescence
         * @return Elément de l'arborescence contenant la nouvelle musique
         */
        virtual SongListItem* addNewSong(Constants::SongList_t list, const QString& filePath, SongListItem *parentDir = 0);

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
         * @param list Liste à laquelle on veut ajouter les musiques
         * @param songs Arborescence de musiques à ajouter
         */
        virtual void addSongs(Constants::SongList_t list, SongTreeRoot *songs);

        /**
         * @brief Lance la première musique du player.
         */
        virtual void firstSong();

        /**
         * @brief Lance la musique précédente du player.
         */
        virtual void previousSong();

        /**
         * @brief Lance la musique suivante du player.
         */
        virtual void nextSong();

        /**
         * @brief Met à jour Fmod et la lecture de musique en changeant si besoin.
         */
        virtual void update();

    public slots:

        /**
         * @brief Lance la musique d'indice song.
         * @param song Indice de la musique
         * @return true si la musique a bien été modifiée
         */
        virtual bool changeSong(int song);

        /**
         * @brief Traite la commande passée en paramètre et émet la réponse une fois terminée.
         * @param command Commande à traiter
         */
        virtual void executeNetworkCommand(CommandRequest *command);
};

#endif  // __PLAYER_H__
