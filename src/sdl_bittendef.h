/*
* MIT License
*
* Copyright (c) 2023 bitten2up
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
//#define debug
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
// TODO make this editable in an settings menu
#define SCREENWIDTH 800
#define SCREENHEIGHT 450
#define SCREENFPS 60;

////////////////////////////////////////////////////////////
// game version
////////////////////////////////////////////////////////////
#define BIT_VERSION 0x01

////////////////////////////////////////////////////////////
// tmx layers with names emum
////////////////////////////////////////////////////////////
typedef enum {
    BACKGROUND_LAYER    = 1,
    CHESTS_LAYER        = 3
} BittenLayers;

#endif
