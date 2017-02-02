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

#include <QFile>

#include "FmodManager.h"
#include "../Util/composedmap.h"
#include "../Constants.h"
#include "../Gui/SongListItem.h"


namespace network
{
    class RemoteSong;

    namespace commands
    {
        class CommandRequest;
        class CommandReply;
    }
}


namespace audio {


#define FIRST_SONG      mp_Songs.begin()
#define UNDEFINED_SONG  mp_Songs.end()

class Song;


class Player : public QObject
{
    Q_OBJECT

    public:

        using SongId = unsigned int;

    private:

        using SongList = util::ComposedMap<SongList_t, std::list<std::shared_ptr<Song>>>;
        using SongIt = SongList::const_iterator;

        unsigned int m_Cpt;

        SongList mp_Songs;
        SongIt m_CurrentSong;

        bool m_Playlist;
        bool m_Loop;

        bool m_Pause;
        bool m_Stop;

        bool m_Mute;
        int m_VolumeState;

        QFile clientFile;

        std::unique_ptr<SoundID_t> mp_PreviewId;


        /**
         * @brief Génère un nouvel identifiant pour une musique.
         * @return Identifiant généré
         */
        SongId getNewSongId();

        /**
         * @brief Récupère la musique du fichier passé en paramètre si elle est dans la liste.
         * @param filePath Chemin du fichier à chercher
         * @return Musique si elle existe, nullptr sinon
         */
        std::shared_ptr<Song> getLocalSong(const QString& filePath) const;

        /**
         * @brief Récupère la musique distante d'identifiant passé en paramètre si elle est dans la liste.
         * @param id Identifiant de la musique distante
         * @return Musique si elle existe, nullptr sinon
         */
        std::shared_ptr<network::RemoteSong> getRemoteSong(const SongId id) const;

        /**
         * @brief Créé un nouveau son local s'il n'existe pas encore à partir du chemin passé en paramètre.
         * @param pos Position à laquelle le nouveau son est ajouté
         * @param filePath Chemin du son à créer
         * @param inFolder Présence du son dans le dossier ou non
         * @return Objet son créé, nullptr sinon
         */
        std::shared_ptr<Song> createLocalSong(SongList::mapped_type::const_iterator& pos, const QString& filePath, bool inFolder);

        /**
         * @brief Ajoute une nouvelle musique dans la liste du player.
         * @param list Liste à laquelle on veut ajouter la musique
         * @param pos Position à laquelle on veut ajouter la musique
         * @param filePath Chemin du fichier à ajouter
         * @param parentDir Parent dans l'arborescence
         * @return Elément de l'arborescence contenant la nouvelle musique
         */
        gui::SongListItem* addNewSong(SongList_t list, SongList::mapped_type::const_iterator& pos, const QString& filePath, gui::SongListItem *parentDir = nullptr);

        /**
         * @brief Recherche une musique à partir de son identifiant.
         * @param songId Identifiant de la musique
         * @return Itérateur sur le son correspondant à l'identifiant passé en paramètre (end si pas trouvé)
         */
        SongIt findSong(SongId songId) const;

        /**
         * @brief Retourne la première musique de la liste.
         * @return Position de la première musique
         */
        SongIt first() const;

        /**
         * @brief Retourne la musique précédente (la dernière si boucle).
         * @return Position de la musique précédente
         */
        SongIt prev() const;

        /**
         * @brief Retourne la musique suivante (la première si boucle).
         * @return Position de la musique suivante
         */
        SongIt next() const;

        /**
         * @brief Cherche la musique précédente avec la disponibilité indiquée.
         * @param available Disponibilité minimale de la musique cherchée
         * @return Position de la musique précédente
         */
        SongIt findPrevSong(bool available);

        /**
         * @brief Cherche la musique suivante avec la disponibilité indiquée.
         * @param available Disponibilité minimale de la musique cherchée
         * @return Position de la musique suivante
         */
        SongIt findNextSong(bool available);

        /**
         * @brief Lance la musique passée en paramètre.
         * @param song Itérateur sur la musique
         * @return true si la musique a bien été modifiée
         */
        bool changeSong(SongIt song);

    signals:

        /**
         * @brief Signal émis lorsque le son courant change.
         */
        void songChanged();

        /**
         * @brief Signal émis lorsque le player change d'état.
         * @param state Nouvel état du player
         */
        void stateChanged(PlayerState state);

        /**
         * @brief Signal émis lorsqu'une musique n'a pas pu être ouverte.
         * @param songId Identifiant de la musique
         */
        void streamError(audio::Player::SongId songId);

        /**
         * @brief Signal émis lorsque la preview est terminée.
         */
        void previewFinished();

        /**
         * @brief Signal émis lorsque la commande reçue a été traitée et que la réponse est créée.
         * @param reply Réponse à la requête exécutée
         */
        void commandExecuted(std::shared_ptr<network::commands::CommandReply> reply);

    public:

        Player();
        ~Player();

        /**
         * @brief getCurrentSong
         * @return Pointeur vers le son actuel.
         */
        std::shared_ptr<Song> getCurrentSong();

        /**
         * @brief Compte le nombre de musiques de la liste passée en paramètre.
         * @param list Liste dont on veut le nombre d'éléments
         * @return Nombre de musiques enregistrées.
         */
        int songsCount(SongList_t list = SongList_t::ALL_SONGS) const;

        /**
         * @brief Active la lecture.
         */
        void play();

        /**
         * @brief Stoppe le player.
         */
        void stop();

        /**
         * @brief Met en pause le player.
         */
        void pause();

        /**
         * @brief Change l'état mute du player (mute/unmute).
         * @param mute Etat mute à appliquer
         */
        void mute(bool mute);

        /**
         * @brief isPlaying
         * @return true si ni pause, ni stop.
         */
        bool isPlaying() const;

        /**
         * @brief isStopped
         * @return true si le player est stoppé.
         */
        bool isStopped() const;

        /**
         * @brief isPaused
         * @return true si le player est en pause.
         */
        bool isPaused() const;

        /**
         * @brief isMuted
         * @return true si le player est mute.
         */
        bool isMuted() const;

        /**
         * @brief isLoop
         * @return true si la boucle est activée.
         */
        bool isLoop() const;

        /**
         * @brief Applique ou retire l'état loop au player.
         * @param loop Valeur à affecter à l'attribut Loop.
         */
        void setLoop(bool loop);

        /**
         * @brief getVolumeState
         * @return Etat du volume.
         */
        int getVolumeState() const;

        /**
         * @brief Modifie le volume du player.
         * @param volumeState Etat du volume
         */
        void setVolume(int volumeState);

        /**
         * @brief Vide la liste des musiques du player à partir de l'indice de la listte passé en paramètre.
         * @param list Liste dont on veut supprimer les éléments
         */
        void clearSongs(SongList_t list = SongList_t::ALL_SONGS);

        /**
         * @brief Créé un nouveau son distant s'il n'existe pas encore à partir des infos passées en paramètre.
         * @param file Chemin du fichier
         * @param remoteId Identifiant distant du son
         * @param length Longueur du son
         * @param artist Artiste du son
         * @param settings Paramètres de lecture du son distant
         * @return Objet son créé, nullptr sinon
         */
        std::shared_ptr<network::RemoteSong> createRemoteSong(const QString& file, SongId remoteId, SoundPos_t length, const QString& artist, SoundSettings *settings);

        /**
         * @brief Ajoute une nouvelle musique dans la liste du player.
         * @param list Liste à laquelle on veut ajouter la musique
         * @param filePath Chemin du fichier à ajouter
         * @param parentDir Parent dans l'arborescence
         * @return Elément de l'arborescence contenant la nouvelle musique
         */
        gui::SongListItem* addNewSong(SongList_t list, const QString& filePath, gui::SongListItem *parentDir = nullptr);

        /**
         * @brief Remplit le vecteur Musiques à partir des fichiers
         *        contenus dans le répertoire donné en paramètre.
         * @param pos Position à laquelle les musiques du répertoire sont ajoutées
         * @param dirPath Répertoire à parcourir
         * @param parentDir Parent dans l'arborescence
         * @return Arborescence des fichiers lus
         */
        gui::SongTreeRoot* loadSongs(SongList::mapped_type::const_iterator& pos, const QString& dirPath, gui::SongTreeRoot *parentDir = nullptr);

        /**
         * @brief Recharge la liste des musiques locales avec le répertoire passé en paramètre.
         * @param dirPath Répertoire à parcourir
         * @return Arborescence des fichiers lus
         */
        gui::SongTreeRoot* reloadSongs(const QString& dirPath);

        /**
         * @brief Lance la première musique du player.
         */
        void firstSong();

        /**
         * @brief Lance la musique précédente du player.
         */
        void previousSong();

        /**
         * @brief Lance la musique suivante du player.
         */
        void nextSong();

        /**
         * @brief Met à jour Fmod et la lecture de musique en changeant si besoin.
         */
        void update();

        /**
         * @brief Vérifie si le lecteur prévisualise une musique.
         * @return true si une prévisualisation a lieu
         */
        bool isPreviewing() const;

        /**
         * @brief Retourne la position de la chanson prévisualisée.
         * @return Position courante de la prévisualisation
         */
        SoundPos_t getPreviewPosition() const;

        /**
         * @brief Retourne la longueur de la chanson prévisualisée.
         * @return Longueur de la chanson prévisualisée
         */
        SoundPos_t getPreviewLength() const;

        /**
         * @brief Démarre la preview de la chanson passée en paramètre.
         * @param filePath Chemin de la chanson à lancer
         */
        void startPreview(const QString& filePath);

        /**
         * @brief Arrête la preview et poursuit la lecture mise en pause.
         */
        void stopPreview();

    public slots:

        /**
         * @brief Lance la musique d'indice song.
         * @param songId Indice de la musique
         * @return true si la musique a bien été modifiée
         */
        bool changeSong(SongId songId);

        /**
         * @brief Supprime la musique d'indice songId.
         * @param songId Indice de la musique
         */
        void removeSong(SongId songId);

        /**
         * @brief Traite la commande passée en paramètre et émet la réponse une fois terminée.
         * @param command Commande à traiter
         */
        void executeNetworkCommand(std::shared_ptr<network::commands::CommandRequest> command);
};


} // audio

#endif  // __PLAYER_H__
