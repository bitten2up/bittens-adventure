/**
* this file toggles debugging and release flags for the game
*/
#ifndef BITTENDEF_H
#define BITTENDEF_H
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
//#include "raylib.h"
#include "tmx.h"
// set the title of the game
#define GAME_NAME "bittens adventure"
// enables debugging features, disable this for releases
#define debug
#ifdef WIN32
#define PATCH_DLL "patch.dll"
#else
#define PATCH_DLL "libbit-patch.so"
#endif
//#define DISCORD // Enables discord rpc, use MAKE DISCORDRPC=TRUE to compile with this flag

// files
#ifndef PLATFORM_WEB
#define SAVE_FILE "bitten.sav"
#endif
#ifdef PLATFORM_WEB
#define SAVE_FILE "assets/bitten.sav"
#endif
// TODO make this editable in an settings menu
#define SCREENWIDTH 800
#define SCREENHEIGHT 450
#define SCREENFPS 60;

////////////////////////////////////////////////////////////
// game version
////////////////////////////////////////////////////////////
#define BIT_VERSION 0x01
////////////////////////////////////////////////////////////
// Game storage enum
////////////////////////////////////////////////////////////
typedef enum {
    MUSIC    = 0,
    SAVEDX   = 1,
    SAVEDY   = 2
} SaveData;

////////////////////////////////////////////////////////////
// tmx layers with names emum
////////////////////////////////////////////////////////////
typedef enum {
    BACKGROUND_LAYER    = 1,
    CHESTS_LAYER        = 3
} BittenLayers;

//////////////////////////////////////////////////////////////
// Structs and enums, defined here for ease of use
// it is a pain to do it any other way even if they are safer
//////////////////////////////////////////////////////////////
#endif
