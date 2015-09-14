/*************************************
 * @file    FmodManager.h
 * @date    29/07/14
 * @author  Manuel
 *
 * Déclarations de la classe Fmod
 * contenant l'ensemble de la gestion
 * des fonctions fmodex.
 *************************************
*/

#ifndef __FMODMANAGER_H__
#define __FMODMANAGER_H__

#include <fmod.h>
#include <fmod_errors.h>
#include <vector>
#include <string>

#include "Constants.h"
#include <QPixmap>

#define VOLUME_MIN    0.0
#define VOLUME_MAX    1.0

typedef unsigned int  SoundID_t;
typedef unsigned int  SoundPos_t;   // Position en ms

typedef struct
{
    FMOD_FILE_OPENCALLBACK openCallback;
    FMOD_FILE_CLOSECALLBACK closeCallback;
    FMOD_FILE_READCALLBACK readCallback;
    FMOD_FILE_SEEKCALLBACK seekCallback;

} SoundSettings;

class FmodManager
{
    private:

        FMOD_SYSTEM *mp_System;

        std::vector<FMOD_CHANNEL*> mp_Channels;

        std::vector<FMOD_SOUND*> mp_Sounds;

        FMOD_CHANNELGROUP *mp_ChannelGroup;


        /* Instance du singleton */
        static FmodManager *mp_Instance;


        FmodManager(int maxChannels = MAX_CHANNELS_NB);
        virtual ~FmodManager();

        /**
         * Attribue un SoundID libre.
         * @return id attribué
        */
        virtual SoundID_t getSoundID() const;

        /**
         * Libère la mémoire du son chargé.
         * @param id Identifiant du son à libérer.
        */
        virtual void releaseSound(SoundID_t id);

        /**
         * Vérifie si le canal associé à l'id est utilisé ou non.
         * @param id Identifiant du canal à vérifier
         * @return true si le canal est utilisé, false sinon.
         */
        virtual bool isChannelUsed(SoundID_t id) const;

    public:

        enum StreamError_t { FILE_ERROR, FORMAT_ERROR };

        /**
         * Créé le singleton s'il n'existe pas
         * et retourne l'instance correspondante.
         * @return instance du singleton
        */
        static FmodManager* getInstance();

        /**
         * Détruit le singleton alloué dynamiquement.
        */
        static void deleteInstance();

        /**
         * Ouvre le fichier son passé en paramètre.
         * @param soundFile Fichier à ouvrir
         * @param settings Options de chargement de la musique (callbacks utilisés)
         * @return identifiant du canal associé
        */
        virtual SoundID_t openFromFile(const std::string& soundFile, SoundSettings *settings = 0) throw (StreamError_t);

        /**
         * Joue le son chargé.
         * @param id Identifiant du son à jouer
        */
        virtual void playSound(SoundID_t id);

        /**
         * Arrête le son joué sur le canal id.
         * @param id Identifiant du canal à stopper
        */
        virtual void stopSound(SoundID_t id);

        /**
         * Met ou retire la pause du canal id.
         * @param id Identifiant du canal à modifier.
         * @param paused Etat pause à mettre.
        */
        virtual void pauseSound(SoundID_t id, bool paused) const;

        /**
         * @param id Identifiant du son à mesurer
         * @return durée de la musique (ms).
        */
        virtual SoundPos_t getSoundLength(SoundID_t id) const;

        /**
         * @param id Identifiant du canal à tester
         * @return position courante de la musique jouée (ms).
        */
        virtual SoundPos_t getSoundPosition(SoundID_t id) const;

        /**
         * Change la position de la musique.
         * @param id Identifiant du canal à modifier
         * @param pos Position à appliquer
        */
        virtual void setSoundPosition(SoundID_t id, SoundPos_t pos);

        /**
         * @param id Identifiant du canal à tester
         * @return true si le canal id est en train de jouer.
        */
        virtual bool isPlaying(SoundID_t id) const;

        /**
         * Récupère le spectre du son joué dans le canal.
         * @param id Identifiant du canal à tester
         * @param values Tableau dans lequel sont stockées
         *               les valeurs.
         * @return Tableau dans lequel sont stockées les valeurs.
        */
        virtual float* getChannelSpectrum(SoundID_t id, float *values) const;

        /**
         * Récupère le volume du canal.
         * @param id Identifiant du canal
         * @return Volume du canal
        */
        virtual float getVolume(SoundID_t id) const;

        /**
         * Modifie le volume du canal.
         * @param id Identifiant du canal à modifier
         * @param volume Volume à appliquer
        */
        virtual void setVolume(SoundID_t id, float volume) const;

        /**
         * Modifie le volume de l'ensemble des canaux.
         * @param volume Volume à appliquer
        */
        virtual void setVolume(float volume) const;

        /**
         * Change l'état mute de l'ensemble des canaux.
         * @param mute Etat à appliquer
        */
        virtual void setMute(bool mute) const;

        /**
         * @param id Identifiant du canal dont on veut le tag
         * @param tagName Tag que l'on cherche
         * @return Tag du son, "" si pas de tag.
         */
        virtual std::string getSongTag(SoundID_t id, const std::string& tagName) const;

        /**
         * @brief Récupère les données de la pochette du mp3 dans les tags.
         * @param id Identifiant du canal dont on veut le tag
         * @param dataLength Pointeur vers la longueur du tag
         * @return Données de l'image
         */
        virtual char* getSongPictureData(SoundID_t id, unsigned int *dataLength = 0) const;
};

#endif  // __FMODMANAGER_H__
