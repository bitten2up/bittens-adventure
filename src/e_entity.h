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
///////////////////////////////////
// structs for enemies and players
///////////////////////////////////

#ifndef E_ENTITY_H
#define E_ENTITY_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

typedef struct e_entitySprite e_entitySprite;
typedef struct e_player e_player;

struct e_entitySprite {
	SDL_Texture* sprite;
	SDL_Rect src;
	SDL_Rect dst;
};
struct e_direction {
  int8_t left;
  int8_t right;
  int8_t up;
  int8_t down;
};

struct e_player{
	e_entitySprite entitySprite;
	int8_t name[10];
	struct e_direction direction;
	int32_t x;
	int32_t y;
};

#endif
