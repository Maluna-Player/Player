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
#define WINDOW_HEIGHT       600

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

#define SPECTRUM_X          200
#define SPECTRUM_Y          0


/*******************************
/** Paramètres des boutons
/*******************************/

// Liste des boutons
enum { PLAY_BUTTON, PAUSE_BUTTON, STOP_BUTTON, PREV_BUTTON, NEXT_BUTTON, NB_BUTTONS };

// Taille d'un bouton
#define BUTTON_SIZE         40

// Coordonnées des boutons
#define PLAY_X              380
#define PLAY_Y              450
#define STOP_X              380
#define STOP_Y              550
#define PREV_X              320
#define PREV_Y              500
#define NEXT_X              440
#define NEXT_Y              500

#endif  // __CONSTANTS_HPP__
