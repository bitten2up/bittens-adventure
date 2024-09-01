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

#include "sdl_bittendef.h"
#include "p_player.h"
#include "e_entity.h"
#include "e_collision.h"

// overworld shit

void p_moveOverworld()
{
  bitgame.player.y+=bitgame.player.direction.up - bitgame.player.direction.down;
  if (checkCollision(bitgame.map, (bitgame.map->width/2)-((bitgame.player.x)/32)-5, (bitgame.map->height/2)-((bitgame.player.y+8)/32)) == CHESTS_LAYER)
  {
    bitgame.player.y -= bitgame.player.direction.up - bitgame.player.direction.down;
    //bitgame.state = platformer;
    #if 0
    bitgame.player.entitySprite.dst.x = SCREENWIDTH/4;
    bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
    bitgame.player.entitySprite.dst.w = 32;
    bitgame.player.entitySprite.dst.h = 32;
    #endif
  }
  bitgame.player.x += bitgame.player.direction.left - bitgame.player.direction.right;
  if (checkCollision(bitgame.map, (bitgame.map->width/2)-((bitgame.player.x)/32)-5, (bitgame.map->height/2)-((bitgame.player.y+8)/32)) == CHESTS_LAYER)
  {
    bitgame.player.x -= bitgame.player.direction.left - bitgame.player.direction.right;
    //bitgame.state = platformer;
    #if 0
    bitgame.player.entitySprite.dst.x = SCREENWIDTH/4;
    bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
    bitgame.player.entitySprite.dst.w = 32;
    bitgame.player.entitySprite.dst.h = 32;
    #endif
  }
}

void p_enterOverworld()
{
  bitgame.player.entitySprite.dst.x = SCREENWIDTH/2;
  bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
  bitgame.player.entitySprite.dst.w = 32;
  bitgame.player.entitySprite.dst.h = 32;
  bitgame.state = overworld;
}

// platformer shit
//

int8_t airtime = 0;
static void p_jump()
{
  if (airtime < 5)
  {
    bitgame.player.voly = bitgame.player.direction.up * 20 - bitgame.player.gravity;
    airtime++;
  }
  else if (airtime < 20)
    airtime++;
  else
    bitgame.player.voly = -bitgame.player.gravity * (airtime-5);
  bitgame.player.y += bitgame.player.voly;
}

void p_movePlatformer()
{

  p_jump();

  if (checkCollision(bitgame.map, (bitgame.map->width/2)-((bitgame.player.x)/32)-5, (bitgame.map->height/2)-((bitgame.player.y+8)/32)) == CHESTS_LAYER)
  {
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


  bitgame.player.x += (bitgame.player.direction.left - bitgame.player.direction.right)*5;

  if (checkCollision(bitgame.map, (bitgame.map->width/2)-((bitgame.player.x)/32), (bitgame.map->height/2)-((bitgame.player.y)/32)) == CHESTS_LAYER)
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
