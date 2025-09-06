/*
* MIT License
*
* Copyright (c) 2021-2025 bitten2up
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

typedef struct {
	SDL_Texture* sprite;
	SDL_Rect src;
	SDL_Rect dst;
} e_sprite ;

struct p_inputs {
  int8_t left;
  int8_t right;
  int8_t up;
  int8_t down;
  int8_t x;
};

enum e_state {
	grounded,
};

#define defaultGravity 1

struct e_player
{
	// x and y for later conversions
	int32_t x;
	int32_t y;

	int8_t gravity;
	uint8_t angle;
	int8_t voly;

	struct p_inputs inputs;
	enum e_state state;
	e_sprite entitySprite;
};

typedef struct {
	char name[256];
	e_sprite entitySprite;
	uint16_t health;
} e_enemy;


#endif
