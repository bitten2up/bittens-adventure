/**
* this file toggles debugging and release flags for the game
*/
#ifndef bittendef
#define bittendef
#include <stdbool.h>
// set the title of the game
#define GAME_NAME "bittens adventure"
// enables debugging features, disable this for releases
#define debug
//#define debugsprites // cuts fps to 15fps, sense i animate at 15fps then add extra frames
// files
#ifndef PLATFORM_WEB
#define SETTINGS_FILE "bitten.sav"
#endif
#ifdef PLATFORM_WEB
#define SETTINGS_FILE "assets/bitten.sav"
#endif
// TODO make this editable in an settings menu
#define SCREENWIDTH 800
#define SCREENHEIGHT 450
//#include <stdbool.h>
////////////////////////////////////////////////////////////
// Game storage
////////////////////////////////////////////////////////////
typedef enum {
    MUSIC    = 0,
    SAVEDX   = 1,
    SAVEDY   = 2
} SaveData;

////////////////////////////////////////////////////////////
// tmx layers with names
////////////////////////////////////////////////////////////
typedef enum {
    BACKGROUND_LAYER    = 1,
    CHESTS_LAYER        = 2
} BittenLayers;


////////////////////////////////////////////////////////////
// Structs, defined here for ease of use
////////////////////////////////////////////////////////////
typedef struct _bit_enemy bit_enemy;
typedef struct _bit_player bit_player;
typedef struct _bit_settings bit_settings; // to be implemented

struct _bit_enemy {
    char name[20];
    float hp;
};

struct _bit_player {
    int health;
    char name[20];
    char pronouns[10];
};

struct _bit_settings {
    int width;
    int height;
    bool audio;
    bool modded;
    bool silent; // dont show modded text durring gameplay
};

//#define bit_enemy struct bit_enemy

//#define DISCORD // Enables discord rpc, use MAKE DISCORDRPC=TRUE to compile with this flag, also from my testing you need to compile it twice, once with CC=gcc then with CC=g++ due to a linking error and not being cpp compatible
#endif