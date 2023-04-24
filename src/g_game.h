#ifndef G_GAME_H
#define G_GAME_H
#include "e_entity.h"
#include <tmx.h>

typedef struct bit_game bit_game;
typedef struct g_settings g_settings;
typedef enum {
    title,
    overworld,
    battle,
} g_state;
struct g_settings
{
    int width;
    int height;
    bool audio;
    bool modded;
    bool silent; // dont show modded text durring gameplay
};

struct g_game
{
    g_settings settings;
    e_player player;
    //bit_enemy enemy;
    g_state state;
    //bit_battleAni battleAni;
    tmx_map* map;
    bool invalidSave;
};
#endif
