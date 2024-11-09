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

//----------------------------------------------------------
// p_player.c
/***********************************************************
* this file handles the movement of the player
************************************************************/

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
// ENGINE HEADERS
////////////////////////////////////////////////////////////

#include "sdl_bittendef.h"
#include "p_player.h"
#include "e_entity.h"
#include "e_collision.h"

// overworld shit

void p_moveOverworld()
{
  bitgame.player.y+=bitgame.player.direction.up - bitgame.player.direction.down;
  if (checkCollision(bitgame.map, (bitgame.map->width/2)-((bitgame.player.x-16)/32), (bitgame.map->height/2)-((bitgame.player.y-16)/32)) == CHESTS_LAYER)
  {
    bitgame.player.y -= bitgame.player.direction.up - bitgame.player.direction.down;
    bitgame.state = battle;
    bitgame.player.entitySprite.dst.x = SCREENWIDTH/4;
    bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
    bitgame.player.entitySprite.dst.w = 32;
    bitgame.player.entitySprite.dst.h = 32;
  }
  bitgame.player.x += bitgame.player.direction.left - bitgame.player.direction.right;
  if (checkCollision(bitgame.map, (bitgame.map->width/2)-((bitgame.player.x-16)/32), (bitgame.map->height/2)-((bitgame.player.y-16)/32)) == CHESTS_LAYER)
  {
    bitgame.player.x -= bitgame.player.direction.left - bitgame.player.direction.right;
    bitgame.state = battle;
    bitgame.player.entitySprite.dst.x = SCREENWIDTH/4;
    bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
    bitgame.player.entitySprite.dst.w = 32;
    bitgame.player.entitySprite.dst.h = 32;
  }
}

void p_enterOverworld()
{
  bitgame.player.entitySprite.dst.x = SCREENWIDTH/2-16;
  bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
  bitgame.player.entitySprite.dst.w = 32;
  bitgame.player.entitySprite.dst.h = 32;
  bitgame.state = overworld;
}

// platformer shit
//

uint8_t airtime = 0;
#if 1
static void p_jump()
{
  if (airtime < 5)
  {
    bitgame.player.voly = bitgame.player.direction.up * (20 * bitgame.player.gravity);
  }
  else if (airtime < 20) {}
  else
    bitgame.player.voly = -bitgame.player.gravity * (airtime-5);
  bitgame.player.y += bitgame.player.voly;
  airtime++;
  printf("airtime:%i\n",airtime);
}
#else
#define square(x) (x)*(x)
static void p_jump()
{
  static bool jumping = false;
  if (airtime == 0)
  {
    if (bitgame.player.direction.up == 1)
      jumping = true;
    else
      jumping = false;

  }


  if (jumping)
  {
    airtime++;
    bitgame.player.voly = square(-bitgame.player.gravity*airtime) + (12*airtime);
  }

}
#endif

void p_movePlatformer()
{

  p_jump();

  if (checkCollision(bitgame.map, (bitgame.map->width/2)-((bitgame.player.x-16)/32), (bitgame.map->height/2)-((bitgame.player.y-16)/32)) == CHESTS_LAYER)
  {
    printf("airtime:%i\n",airtime);
    bitgame.player.y -= bitgame.player.voly;
    bitgame.player.direction.up = 0;
    bitgame.player.voly = 0;
    airtime = 0;

  #if 0
    bitgame.state = battle;
    bitgame.player.entitySprite.dst.x = SCREENWIDTH/4;
    bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
    bitgame.player.entitySprite.dst.w = 32;
    bitgame.player.entitySprite.dst.h = 32;
  #endif
  }


  bitgame.player.x += (bitgame.player.direction.left - bitgame.player.direction.right);

  if (checkCollision(bitgame.map, (bitgame.map->width/2)-((bitgame.player.x-16)/32), (bitgame.map->height/2)-((bitgame.player.y-16)/32)) == CHESTS_LAYER)
  {
    bitgame.player.x -= bitgame.player.direction.left - bitgame.player.direction.right;
  #if 0
    bitgame.state = platformer;
    bitgame.player.entitySprite.dst.x = SCREENWIDTH/4;
    bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
    bitgame.player.entitySprite.dst.w = 32;
    bitgame.player.entitySprite.dst.h = 32;
  #endif
  }
}
