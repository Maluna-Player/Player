/*************************************
 * @file    constants.h
 * @date    27/07/14
 * @author  Manuel
 *
 * Contient l'ensemble des constantes
 * de préprocesseur du projet.
 *************************************
*/

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__


#define REFRESH_TIME_MS     50


/*******************************
/** Chemins des ressources
/*******************************/

#define SONGS_SUBDIR        "../resources/Musiques/"
#define FONT_FILE           "../resources/lsans.ttf"
#define IMAGES_SUBDIR       QString("../resources/Images/")
#define BUTTONS_SUBDIR      IMAGES_SUBDIR + "Buttons/"


/*******************************
/** Fenêtre de l'application
/*******************************/

// Taille de la fenêtre du player
#define WINDOW_WIDTH            800
#define WINDOW_HEIGHT           (SPECTRUM_HEIGHT + BOTTOM_BACKGROUND_H)

// Titre de la fenêtre
#define WINDOW_TITLE            "Player"

// Background du bas
#define BOTTOM_BACKGROUND_H     150


/*******************************
/** Paramètres de FMOD
/*******************************/

// Nombre max de canaux
#define MAX_CHANNELS_NB     8

// Nombre d'états du volume
#define NB_VOLUME_STATES    4
#define MUTE_STATE          NB_VOLUME_STATES

#define MOVE_INTERVAL                 1     // sec
#define BUTTON_ACTIVATION_DELAY       10

/*******************************
/** Paramètres du spectre
/*******************************/

#define SPECTRUM_WIDTH      512
#define SPECTRUM_HEIGHT     450
#define SPECTRUM_RATIO      15


/*******************************
/** Position des éléments
/*******************************/

// Taille d'un bouton
#define BUTTON_SIZE         45

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

// Onglet
#define TAB_HEAD_W                      40
#define TAB_HEAD_H                      77
#define TAB_CONTENT_W                   300
#define TAB_CONTENT_H                   300

#define CLOSED_TAB_HEAD_X               (WINDOW_WIDTH - TAB_HEAD_W)
#define OPENED_TAB_HEAD_X               (CLOSED_TAB_HEAD_X - TAB_CONTENT_W)

#define TAB_HEAD_X                      CLOSED_TAB_HEAD_X
#define TAB_HEAD_Y                      45
#define TAB_CONTENT_X                   (TAB_HEAD_X + TAB_HEAD_W)
#define TAB_CONTENT_Y                   (TAB_HEAD_Y - 10)

#define TAB_MOVING_INTERVAL             10

// Liste des musiques
#define SONG_LIST_X                     10
#define SONG_LIST_Y                     10
#define LENGTH_LIST_X                   250

#define SONG_LIST_W                     (TAB_CONTENT_W - SONG_LIST_X)
#define SONG_LIST_H                     (TAB_CONTENT_H - SONG_LIST_Y * 2)

#define SONG_TITLE_H                    25
#define TITLE_LIST_MAX_WIDTH            30
#define TEXT_HEIGHT_SPACE               4

#define SCROLL_INTERVAL                 10


/*******************************
/** Liste des éléments
/*******************************/

// Liste des textures générales
enum { BUTTONS_TEXTURE, BOTTOM_BACKGROUND, NB_TEXTURES };

// Liste des textes
enum { TITLE_TEXT, POS_TEXT, LENGTH_TEXT, NB_TEXTS };

// Liste des objets cliquables
enum Clickable_t { VOLUME_MORE_BUTTON, VOLUME_LESS_BUTTON,
                   REFRESH_DIR_BUTTON, NB_BUTTONS,

                   PROGRESS_BACKGROUND = NB_BUTTONS,
                   VOLUME_VIEWER,
                   SONG_LIST,
                   TAB,
                   NB_CLICKABLES, UNDEFINED_CLICKABLE = NB_CLICKABLES
                 };

// Liste des objets déplaçables
enum Movable_t { PROGRESSBAR, NB_MOVABLES };

// Etats du player
enum State_t { PLAY_STATE, PAUSE_STATE, STOP_STATE };

#endif  // __CONSTANTS_H__
