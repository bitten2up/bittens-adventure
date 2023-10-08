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

#include "sdl_bittendef.h"
#include "p_player.h"
#include "e_entity.h"
#include "e_collision.h"

void p_move(g_game* game)
{
  game->player.y+=game->player.direction.up - game->player.direction.down;
  if (checkCollision(game->map, (game->map->width/2)-((game->player.x)/32)-5, (game->map->height/2)-((game->player.y+8)/32)) == CHESTS_LAYER)
  {
    game->player.y -= game->player.direction.up - game->player.direction.down;
    game->state = battle;
    game->player.entitySprite.dst.x = SCREENWIDTH/4;
    game->player.entitySprite.dst.y = SCREENHEIGHT/2;
    game->player.entitySprite.dst.w = 32;
    game->player.entitySprite.dst.h = 32;
  }
  game->player.x += game->player.direction.left - game->player.direction.right;
  if (checkCollision(game->map, (game->map->width/2)-((game->player.x)/32)-5, (game->map->height/2)-((game->player.y+8)/32)) == CHESTS_LAYER)
  {
    game->player.x -= game->player.direction.left - game->player.direction.right;
    game->state = battle;
    game->player.entitySprite.dst.x = SCREENWIDTH/4;
    game->player.entitySprite.dst.y = SCREENHEIGHT/2;
    game->player.entitySprite.dst.w = 32;
    game->player.entitySprite.dst.h = 32;
  }
}

void p_enterOverworld(g_game* game)
{
  game->player.entitySprite.dst.x = SCREENWIDTH/2;
  game->player.entitySprite.dst.y = SCREENHEIGHT/2;
  game->player.entitySprite.dst.w = 32;
  game->player.entitySprite.dst.h = 32;
  game->state = overworld;
}
