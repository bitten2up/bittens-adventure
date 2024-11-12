/*
* MIT License
*
* Copyright (c) 2021-2024 bitten2up
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
#include <tmx.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "e_entity.h"


typedef enum {
    title,
    overworld,
    platformer,
    battle,
} bit_state;

typedef struct
{
    int width;
    int height;
    bool audio;
    bool modded;
    bool silent; // dont show modded text durring gameplay
} bit_settings;

typedef struct
{
  Uint32 startTime; // internal, allows us to get deltaTime
  Uint32 endTime; // internal, allows us to get deltaTime
  Uint32 deltaTime; // time between the last frame
  uint32_t fps; // current fps
  uint32_t timePerFrame; // miliseconds
  bool showFps;
} bit_preformance;

// allows us to have shitty menus

typedef enum
{
	menuNone,
	battleMain,
} bit_curmenu;

typedef struct
{
	int8_t x;
	int8_t y;
} bit_menupos;

typedef struct {
	bit_curmenu curmenu;
	bit_menupos menupos;
} bit_menu;

// global struct

typedef struct
{
    bit_settings settings;
    bit_menu menu;
    e_player player;
    e_enemy enemy;
    bit_state state;
    //bit_battleAni battleAni;
    bit_preformance preformance;
    tmx_map* map;
    bool invalidSave;
    bool gameRunning;
} bit_game;

void bit_main(void);

extern bit_game bitgame;

#ifdef __cplusplus
}
#endif
#endif
