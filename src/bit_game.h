/*
* MIT License
*
* Copyright (c) 2021-2023 bitten2up
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
////////////////
// g_game.h
/***************
 * stores the top level of the bittenstruct, shouldn't be included, unless absolutly nessary
**/ 
#ifndef BIT_GAME_H
#define BIT_GAME_H
#include "e_entity.h"
#include <tmx.h>
#include <stdbool.h>

typedef struct bit_game bit_game;
typedef struct bit_settings bit_settings;
typedef enum {
    title,
    overworld,
    battle,
} bit_state;
struct bit_settings
{
    int width;
    int height;
    bool audio;
    bool modded;
    bool silent; // dont show modded text durring gameplay
};

struct bit_game
{
    bit_settings settings;
    e_player player;
    //bit_enemy enemy;
    bit_state state;
    //bit_battleAni battleAni;
    tmx_map* map;
    bool invalidSave;
    bool gameRunning;
};
void bit_main(void);

extern bit_game bitgame;

#endif
