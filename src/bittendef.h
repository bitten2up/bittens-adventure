/**
* this file toggles debugging and release flags for the game
*/
#ifndef bittendef
#define bittendef
#include <stdbool.h>
#include "raylib.h"
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


////////////////////////////////////////////////////////////
// Structs and enums, defined here for ease of use
////////////////////////////////////////////////////////////
typedef enum   _bit_battleAni bit_battleAni; // what state are we in animation for battle
typedef enum   _bit_state bit_state;
typedef enum   _bit_direction bit_direction;
typedef struct _bit_sprite bit_sprite;
typedef struct _bit_enemy bit_enemy;
typedef struct _bit_player bit_player;
typedef struct _bit_settings bit_settings;
typedef struct _bit_game bit_game; // contains everything for the game

enum _bit_battleAni {
    intro,
    waitInput,
    playerAttack,
    enemyAttack,
    gameover,
};

enum _bit_state {
    title,
    overworld,
    battle,
};
enum _bit_direction{
    blank,
    down,
    left,
    right,
    up,
};

struct _bit_sprite
{
    Texture2D img;
    Rectangle rec;
    Vector2 pos;
    bit_direction direction;
};

struct _bit_enemy
{
    bit_sprite sprite;
    char name[20];
    float health;
};

struct _bit_player
{
    bit_sprite sprite;
    char name[20];
    char pronouns[10];
    float health;
};

struct _bit_settings
{
    int width;
    int height;
    bool audio;
    bool modded;
    bool silent; // dont show modded text durring gameplay
};

struct _bit_game
{
    bit_settings settings;
    bit_player player;
    bit_enemy enemy;
    bit_state state;
    bit_battleAni battleAni;
    int map;
};

//#define bit_enemy struct bit_enemy

//#define DISCORD // Enables discord rpc, use MAKE DISCORDRPC=TRUE to compile with this flag, also from my testing you need to compile it twice, once with CC=gcc then with CC=g++ due to a linking error and not being cpp compatible

// called from main.c
#ifdef DEFINE_MAIN
/*
 player defines
*/
// player sprite
    #define bitten game.player.sprite.img
// player rectangle aka what we see of the sprite
    #define bittenRec game.player.sprite.rec
// position of player
    #define bittenPos game.player.sprite.pos
// direction of player
    #define bittenDirection game.player.sprite.direction
// player health
    #define bittenHealth game.player.health

/*
* enemy defines
*/
// enemy sprite
    #define enemySprite game.enemy.sprite.img
// enemy rectangle aka what we see of the sprite
    #define enemyRec game.enemy.sprite.rec
// position of enemy
    #define enemyPos game.enemy.sprite.pos
// enemy health
    #define enemyHealth game.enemy.health
/*
battle animation defines
*/
    #define battleAni game.battleAni

/*
* gamestate defines
*/
    #define state game.state
    #define isTitle state==title
    #define isBattle state==battle
    #define isOverworld state==overworld
    #define isGameover state==gameover
#else
// not main.c
/*
 player defines
*/
// player sprite
    #define bitten game->player.sprite.img
// player rectangle aka what we see of the sprite
    #define bittenRec game->player.sprite.rec
// position of player
    #define bittenPos game->player.sprite.pos
// player health
    #define bittenHealth game->player.health
/*
* enemy defines
*/
// enemy sprite
    #define enemySprite game->enemy.sprite.img
// enemy rectangle aka what we see of the sprite
    #define enemyRec game->enemy.sprite.rec
// position of enemy
    #define enemyPos game->enemy.sprite.pos
// enemy health
    #define enemyHealth game->enemy.health
/*
battle animation defines
*/
    #define battleAni game->battleAni
/*
* gamestate defines
*/
    #define state game->state
    #define isTitle state==title
    #define isGameover state==gameover
    #define isBattle state==battle
    #define isOverworld state==overworld
#endif
#endif