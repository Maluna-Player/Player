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

#define SONGS_SUBDIR        std::string("resources") + Path::separator() + "Musiques"
#define FONT_FILE           std::string("resources") + Path::separator() + "lsans.ttf"
#define IMAGES_SUBDIR       std::string("resources") + Path::separator() + "Images"


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

// Nombre d'états du volume
#define NB_VOLUME_STATES    4
#define MUTE_STATE          NB_VOLUME_STATES


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

#define VOLUME_M_X          220
#define VOLUME_M_Y          550
#define VOLUME_L_X          220
#define VOLUME_L_Y          595

#define REFRESH_DIR_X       600
#define REFRESH_DIR_Y       580

// Barre de progression
#define PROGRESS_BACKGROUND_HEIGHT      30
#define PROGRESS_BACKGROUND_X           0
#define PROGRESS_BACKGROUND_Y           500
#define PROGRESSBAR_HEIGHT              17
#define PROGRESSBAR_Y                   (PROGRESS_BACKGROUND_Y + 6)

// Temps de la musique
#define POS_TEXT_X                      0
#define POS_TEXT_Y                      530
#define LENGTH_TEXT_X                   750
#define LENGTH_TEXT_Y                   POS_TEXT_Y

// Volume
#define VOLUME_X                        100
#define VOLUME_Y                        550
#define VOLUME_SPRITE_W                 85
#define VOLUME_SPRITE_H                 75


/*******************************
/** Liste des éléments
/*******************************/

// Liste des textures générales
enum { BUTTONS_TEXTURE, NB_TEXTURES };

// Liste des textes
enum { TITLE_TEXT, POS_TEXT, LENGTH_TEXT, NB_TEXTS };

// Liste des objets cliquables
enum Clickable_t { PLAY_BUTTON, STOP_BUTTON, PREV_BUTTON, NEXT_BUTTON,
                   VOLUME_MORE_BUTTON, VOLUME_LESS_BUTTON,
                   REFRESH_DIR_BUTTON, NB_BUTTONS,

                   PROGRESS_BACKGROUND = NB_BUTTONS,
                   VOLUME_VIEWER,
                   NB_CLICKABLES
                 };

// Liste des objets déplaçables
enum Movable_t { PROGRESSBAR, NB_MOVABLES };

// Etats du player
enum State_t { PLAY_STATE, PAUSE_STATE, STOP_STATE };

#endif  // __CONSTANTS_HPP__
