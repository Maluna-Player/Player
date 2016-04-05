/*************************************
 * @file    Constants.h
 * @date    27/07/14
 * @author  Manuel
 *
 * Contient l'ensemble des constantes
 * de préprocesseur du projet.
 *************************************
*/

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <QString>

namespace Constants
{

const unsigned int REFRESH_TIME_MS = 50;


/*******************************
/** Chemins des ressources
/*******************************/

const QString SONGS_SUBDIR          = "../resources/Musiques/";
const QString IMAGES_SUBDIR         = "../resources/Images/";
const QString BUTTONS_SUBDIR        = IMAGES_SUBDIR + "Buttons/";
const QString MENU_SUBDIR           = IMAGES_SUBDIR + "Menu/";


/*******************************
/** Fenêtre de l'application
/*******************************/

// Taille de la fenêtre du player
const unsigned int WINDOW_WIDTH     = 800;
const unsigned int WINDOW_HEIGHT    = 600;

// Titre de la fenêtre
const std::string WINDOW_TITLE      = "Player";


/*******************************
/** Paramètres de FMOD
/*******************************/

// Nombre max de canaux
const unsigned int MAX_CHANNELS_NB  = 8;

// Nombre d'états du volume
const unsigned int NB_VOLUME_STATES = 9;
const unsigned int MUTE_STATE       = NB_VOLUME_STATES;


/*******************************
/** Paramètres du spectre
/*******************************/

const unsigned int SPECTRUM_WIDTH   = 512;
const unsigned int SPECTRUM_HEIGHT  = 400;
const int SPECTRUM_RATIO            = 15;


/*******************************
/** Propriétés des éléments
/*******************************/

// Barre de progression
const unsigned int PROGRESS_BACKGROUND_HEIGHT   = 30;
const unsigned int PROGRESSBAR_HEIGHT           = 23;
const unsigned int LOADBAR_HEIGHT               = 17;

// Volume
const unsigned int VOLUME_SPRITE_W              = 50;
const unsigned int VOLUME_SPRITE_H              = 70;
const unsigned int VOLUME_VALUE_W               = 82;
const unsigned int VOLUME_VALUE_H               = 287;
const unsigned int VOLUME_VIEWER_W              = (VOLUME_SPRITE_W + (VOLUME_SPRITE_H * VOLUME_VALUE_W / VOLUME_VALUE_H) + 10);
const unsigned int VOLUME_VIEWER_H              = VOLUME_SPRITE_H;


// Etats du player
enum PlayerState_t { PLAY_STATE, PAUSE_STATE, STOP_STATE };

// Listes de musiques
enum SongList_t { DIRECTORY_SONGS = 1,
                  IMPORTED_SONGS = 2,
                  LOCAL_SONGS = DIRECTORY_SONGS | IMPORTED_SONGS,
                  REMOTE_SONGS = 4,
                  ALL_SONGS = LOCAL_SONGS | REMOTE_SONGS };

inline SongList_t operator|(SongList_t l1, SongList_t l2)
{
    return static_cast<SongList_t>(static_cast<int>(l1) | static_cast<int>(l2));
}

}

#endif  // __CONSTANTS_H__
