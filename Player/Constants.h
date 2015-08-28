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
#define WINDOW_HEIGHT           600

// Titre de la fenêtre
#define WINDOW_TITLE            "Player"


/*******************************
/** Paramètres de FMOD
/*******************************/

// Nombre max de canaux
#define MAX_CHANNELS_NB     8

// Nombre d'états du volume
#define NB_VOLUME_STATES    9
#define MUTE_STATE          NB_VOLUME_STATES

#define MOVE_INTERVAL                 1     // sec
#define BUTTON_ACTIVATION_DELAY       10

/*******************************
/** Paramètres du spectre
/*******************************/

#define SPECTRUM_WIDTH      512
#define SPECTRUM_HEIGHT     400
#define SPECTRUM_RATIO      15


/*******************************
/** Propriétés des éléments
/*******************************/

// Taille d'un bouton
#define BUTTON_SIZE         45

// Barre de progression
#define PROGRESS_BACKGROUND_HEIGHT      30
#define PROGRESSBAR_HEIGHT              23
#define LOADBAR_HEIGHT                  17

// Volume
#define VOLUME_SPRITE_W                 50
#define VOLUME_SPRITE_H                 70
#define VOLUME_VALUE_W                  82
#define VOLUME_VALUE_H                  287
#define VOLUME_VIEWER_W                 (VOLUME_SPRITE_W + (VOLUME_SPRITE_H * VOLUME_VALUE_W / VOLUME_VALUE_H) + 10)
#define VOLUME_VIEWER_H                 VOLUME_SPRITE_H

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


/*******************************
/** Liste des éléments
/*******************************/

// Liste des objets cliquables
enum Clickable_t { REFRESH_DIR_BUTTON, NB_BUTTONS,

                   TAB,
                   NB_CLICKABLES, UNDEFINED_CLICKABLE = NB_CLICKABLES
                 };

// Etats du player
enum State_t { PLAY_STATE, PAUSE_STATE, STOP_STATE };

#endif  // __CONSTANTS_H__
