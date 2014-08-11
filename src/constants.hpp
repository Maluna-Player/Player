/*************************************
 * @file    constants.hpp
 * @date    27/07/14
 * @author  Manuel
 *
 * Contient l'ensemble des constantes
 * de préprocesseur du projet.
 *************************************
*/

#ifndef __CONSTANTS_HPP__
#define __CONSTANTS_HPP__


#define REFRESH_TIME_MS     50


/*******************************
/** Chemins des ressources
/*******************************/

#define SONGS_SUBDIR        "resources/Musiques"
#define FONT_FILE           "resources/lsans.ttf"
#define IMAGES_SUBDIR       "resources/Images"


/*******************************
/** Fenêtre de l'application
/*******************************/

// Taille de la fenêtre du player
#define WINDOW_WIDTH        800
#define WINDOW_HEIGHT       650

// Titre de la fenêtre
#define WINDOW_TITLE        "Player"


/*******************************
/** Paramètres de FMOD
/*******************************/

// Nombre max de canaux
#define MAX_CHANNELS_NB     8


/*******************************
/** Paramètres du spectre
/*******************************/

#define SPECTRUM_WIDTH      512
#define SPECTRUM_HEIGHT     450
#define SPECTRUM_RATIO      15

#define SPECTRUM_X          250
#define SPECTRUM_Y          40


/*******************************
/** Position des éléments
/*******************************/

// Taille d'un bouton
#define BUTTON_SIZE         40

// Coordonnées des boutons
#define PLAY_X              380
#define PLAY_Y              550
#define STOP_X              380
#define STOP_Y              600
#define PREV_X              320
#define PREV_Y              580
#define NEXT_X              440
#define NEXT_Y              580

// Barre de progression
#define PROGRESS_BACKGROUND_HEIGHT      30
#define PROGRESS_BACKGROUND_X           0
#define PROGRESS_BACKGROUND_Y           500
#define PROGRESSBAR_HEIGHT              17
#define PROGRESSBAR_Y                   (PROGRESS_BACKGROUND_Y + 6)


/*******************************
/** Liste des textures
/*******************************/

// Liste des textures
enum { BUTTONS_TEXTURE, PROGRESS_BACKGROUND_TEXTURE, PROGRESSBAR_TEXTURE, NB_TEXTURES };

// Liste des boutons
enum { PLAY_BUTTON, PAUSE_BUTTON, STOP_BUTTON, PREV_BUTTON, NEXT_BUTTON, NB_BUTTONS };

#endif  // __CONSTANTS_HPP__
