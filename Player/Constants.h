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


constexpr unsigned int REFRESH_TIME_MS = 50;


/*******************************
/** Chemins des ressources
/*******************************/

#define SONGS_SUBDIR                    "../resources/Musiques/"
#define IMAGES_SUBDIR                   "../resources/Images/"
constexpr const char* BUTTONS_SUBDIR    = IMAGES_SUBDIR "Buttons/";
constexpr const char* MENU_SUBDIR       = IMAGES_SUBDIR "Menu/";


/*******************************
/** Fenêtre de l'application
/*******************************/

// Taille de la fenêtre du player
constexpr unsigned int WINDOW_WIDTH     = 800;
constexpr unsigned int WINDOW_HEIGHT    = 600;

// Titre de la fenêtre
constexpr const char* WINDOW_TITLE      = "Player";


/*******************************
/** Paramètres de FMOD
/*******************************/

// Nombre max de canaux
constexpr unsigned int MAX_CHANNELS_NB  = 8;

// Nombre d'états du volume
constexpr unsigned int NB_VOLUME_STATES = 9;
constexpr unsigned int MUTE_STATE       = NB_VOLUME_STATES;


/*******************************
/** Paramètres du spectre
/*******************************/

constexpr unsigned int SPECTRUM_WIDTH   = 512;
constexpr unsigned int SPECTRUM_HEIGHT  = 400;
constexpr int SPECTRUM_RATIO            = 15;


/*******************************
/** Propriétés des éléments
/*******************************/

// Barre de progression
constexpr unsigned int PROGRESS_BACKGROUND_HEIGHT   = 30;
constexpr unsigned int PROGRESSBAR_HEIGHT           = 23;
constexpr unsigned int LOADBAR_HEIGHT               = 17;

// Volume
constexpr unsigned int VOLUME_SPRITE_W              = 50;
constexpr unsigned int VOLUME_SPRITE_H              = 70;
constexpr unsigned int VOLUME_VALUE_W               = 82;
constexpr unsigned int VOLUME_VALUE_H               = 287;
constexpr unsigned int VOLUME_VIEWER_W              = (VOLUME_SPRITE_W + (VOLUME_SPRITE_H * VOLUME_VALUE_W / VOLUME_VALUE_H) + 10);
constexpr unsigned int VOLUME_VIEWER_H              = VOLUME_SPRITE_H;


// Preview (ms)
constexpr unsigned int PREVIEW_DELAY                = 2000;
constexpr unsigned int PREVIEW_LENGTH               = 30000;
constexpr unsigned int PREVIEW_ANIMATION_LENGTH     = 1000;

// Boutons prev/next
constexpr unsigned int BUTTON_DELAY                 = 500;
constexpr unsigned int MOVE_INTERVAL                = 1000;

// SongList/Tab
constexpr unsigned int LIST_WIDTH                   = 290;
constexpr unsigned int HIDDEN_LIST_WIDTH            = 10;
constexpr unsigned int TAB_ANIMATION_LENGTH         = 2000;
constexpr double TAB_ANIMATION_SPEED                = static_cast<double>(LIST_WIDTH - HIDDEN_LIST_WIDTH) / TAB_ANIMATION_LENGTH;


// Etats du player
enum class PlayerState { PLAY, PAUSE, STOP };

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

#endif  // __CONSTANTS_H__
