/**
* this file toggles debugging and release flags for the game
*/
#ifndef bittendef
#define bittendef
// set the title of the game
#define GAME_NAME "bittens adventure"
// enables debugging features, disable this for releases
#define debug
//#define debugsprites // cuts fps to 15fps, sense i animate at 15fps then add extra frames
// files
#define SETTINGS_FILE "bitten.sav"
// TODO make this editable in an settings menu
#define SCREENWIDTH 800
#define SCREENHEIGHT 450

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


//#define DISCORD // Enables discord rpc, use MAKE DISCORDRPC=TRUE to compile with this flag, also from my testing you need to compile it twice, once with CC=gcc then with CC=g++ due to a linking error and not being cpp compatible
#endif