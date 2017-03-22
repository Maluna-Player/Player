/*************************************
 * @file    FmodManager.h
 * @date    29/07/14
 * @author  Manuel
 *
 * Déclarations de la classe Fmod
 * contenant l'ensemble de la gestion
 * des fonctions fmod.
 *************************************
*/

#ifndef __FMODMANAGER_H__
#define __FMODMANAGER_H__

#include <fmod.h>
#include <fmod_errors.h>
#include <vector>
#include <string>

#include "Constants.h"


namespace audio {


constexpr float VOLUME_MIN = 0.0;
constexpr float VOLUME_MAX = 1.0;

using SoundID_t = unsigned int;
using SoundPos_t = unsigned int;    // Position en ms

typedef struct
{
    FMOD_FILE_OPEN_CALLBACK openCallback;
    FMOD_FILE_CLOSE_CALLBACK closeCallback;
    FMOD_FILE_READ_CALLBACK readCallback;
    FMOD_FILE_SEEK_CALLBACK seekCallback;

    void *userdata;
} SoundSettings;

class FmodManager
{
    private:

        FMOD_SYSTEM *mp_System;

        std::vector<FMOD_CHANNEL*> mp_Channels;

        std::vector<FMOD_SOUND*> mp_Sounds;

        std::vector<FMOD_DSP*> mp_Dsps;

        FMOD_CHANNELGROUP *mp_ChannelGroup;


        /* Instance du singleton */
        static FmodManager *mp_Instance;


        FmodManager(int maxChannels = MAX_CHANNELS_NB);
        ~FmodManager();

        /**
         * @brief Attribue un identifiant libre.
         * @param mainCanal true si la musique est ouverte sur le canal principal
         * @return Id attribué
        */
        SoundID_t getSoundID(bool mainCanal) const;

        /**
         * @brief Vérifie si le canal associé à l'identifiant id est utilisé ou non.
         * @param id Identifiant du canal à vérifier
         * @return true si le canal est utilisé, false sinon
         */
        bool isChannelUsed(SoundID_t id) const;

    public:

        enum class StreamError { FILE_ERROR, FORMAT_ERROR };

        /**
         * @brief Créé le singleton s'il n'existe pas
         *        et retourne l'instance correspondante.
         * @return Instance du singleton
        */
        static FmodManager& getInstance();

        /**
         * @brief Détruit le singleton alloué dynamiquement.
        */
        static void deleteInstance();

        /**
         * @brief Met à jour FMOD.
         */
        void update() const;

        /**
         * @brief Ouvre le fichier son passé en paramètre.
         * @param soundFile Fichier à ouvrir
         * @param mainCanal true si la musique est ouverte sur le canal principal
         * @param settings Options de chargement de la musique (callbacks utilisés)
         * @return Identifiant du canal associé
        */
        SoundID_t openFromFile(const std::string& soundFile, bool mainCanal = true, SoundSettings *settings = nullptr) throw (StreamError);

        /**
         * @brief Libère la mémoire du son chargé.
         * @param id Identifiant du son à libérer
        */
        void releaseSound(SoundID_t id);

        /**
         * @brief Ajoute un DSP au son d'identifiant id.
         * @param id Identifiant du son à traiter
         * @param size Taille des données de spectre à récupérer
         */
        void addDSP(SoundID_t id, unsigned int size);

        /**
         * @brief Joue le son chargé.
         * @param id Identifiant du son à jouer
        */
        void playSound(SoundID_t id);

        /**
         * @brief Arrête le son joué sur le canal id.
         * @param id Identifiant du canal à stopper
        */
        void stopSound(SoundID_t id);

        /**
         * @brief Met ou retire la pause du canal id.
         * @param id Identifiant du canal à modifier.
         * @param paused Etat pause à appliquer
        */
        void pauseSound(SoundID_t id, bool paused) const;

        /**
         * @brief Récupère la durée de la musique id.
         * @param id Identifiant du son à mesurer
         * @return Durée de la musique (ms)
        */
        SoundPos_t getSoundLength(SoundID_t id) const;

        /**
         * @brief Récupère la position courante de la musique jouée.
         * @param id Identifiant du canal à tester
         * @return Position de la musique (ms)
        */
        SoundPos_t getSoundPosition(SoundID_t id) const;

        /**
         * @brief Change la position de la musique.
         * @param id Identifiant du canal à modifier
         * @param pos Position à appliquer
        */
        void setSoundPosition(SoundID_t id, SoundPos_t pos);

        /**
         * @brief Détermine si le canal du son passé en paramètre est en lecture.
         * @param id Identifiant du canal à tester
         * @return true si le canal est en train de jouer
        */
        bool isPlaying(SoundID_t id) const;

        /**
         * @brief Récupère le spectre du son joué dans le canal.
         * @param id Identifiant du canal à tester
         * @param values Vecteur dans lequel sont stockées
         *               les valeurs
         * @return Vecteur dans lequel sont stockées les valeurs
        */
        std::vector<float>& getChannelSpectrum(SoundID_t id, std::vector<float>& values) const;

        /**
         * @brief Récupère le volume du canal.
         * @param id Identifiant du canal
         * @return Volume du canal
        */
        float getVolume(SoundID_t id) const;

        /**
         * @brief Modifie le volume du canal.
         * @param id Identifiant du canal à modifier
         * @param volume Volume à appliquer
        */
        void setVolume(SoundID_t id, float volume) const;

        /**
         * @brief Modifie le volume de l'ensemble des canaux.
         * @param volume Volume à appliquer
        */
        void setVolume(float volume) const;

        /**
         * @brief Change l'état mute de l'ensemble des canaux.
         * @param mute Etat à appliquer
        */
        void setMute(bool mute) const;

        /**
         * @brief Récupère les données de la pochette du mp3 dans les tags.
         * @param id Identifiant du canal dont on veut le tag
         * @param dataLength Pointeur vers la longueur du tag
         * @return Données de l'image
         */
        char* getSongPictureData(SoundID_t id, unsigned int *dataLength = nullptr) const;
};


} // audio

#endif  // __FMODMANAGER_H__
