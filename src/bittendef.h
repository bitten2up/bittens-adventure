/**
* this file toggles debugging and release flags for the game
*/
#ifndef BITTENDEF_H
#define BITTENDEF_H
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "raylib.h"
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

typedef struct _bit_sprite bit_sprite;
typedef struct _bit_enemy bit_enemy;
typedef struct _bit_player bit_player;
typedef struct _bit_settings bit_settings;
typedef struct _bit_game bit_game; // contains everything for the game

typedef enum {
    intro,
    waitInput,
    playerAttack,
    enemyAttack,
    gameover,
} bit_battleAni;

typedef enum {
    title,
    overworld,
    battle,
} bit_state;
typedef enum {
    blank,
    down,
    left,
    right,
    up,
} bit_direction;

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
    int16_t x;
    int16_t y;
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
    bool invalidSave;
};


// ease of use macros
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