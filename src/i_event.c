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

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "i_event.h"
#include "g_game.h"
#include "e_collision.h"
#include "p_player.h"

struct direction {
  char left;
  char right;
  char up;
  char down;
};

void move(g_game* game);
SDL_Event event;
void i_poll(g_game* game)
{
  while (SDL_PollEvent(&event))
  {
    switch (event.type) {
      case SDL_QUIT:
        game->gameRunning = false;
        break;
      case SDL_KEYDOWN:
        switch( event.key.keysym.sym ) {
          case SDLK_ESCAPE:
            game->gameRunning = false;
            break;
          case SDLK_UP:
            if (game->state == overworld)
            {
              game->player.direction.up = 1;
              game->player.entitySprite.src.x = 0;
            }
            break;
          case SDLK_DOWN:
            if (game->state == overworld){
              game->player.direction.down = 1;
              game->player.entitySprite.src.x = 32;
            }
            break;
          case SDLK_LEFT:
            if (game->state == overworld){
              game->player.direction.left = 1;
              game->player.entitySprite.src.x = 64;
            }
            break;
          case SDLK_RIGHT:
            if (game->state == overworld){
              game->player.direction.right = 1;
              game->player.entitySprite.src.x = 96;
            }
            break;
          case SDLK_RETURN:
            if (game->state == title || game->state == battle)
              p_enterOverworld(game);
            break;
          default:
            break;
        }
        break;
      case SDL_KEYUP:
        switch( event.key.keysym.sym ) {
          case SDLK_DOWN:
            game->player.direction.down = 0;
            break;
          case SDLK_UP:
            game->player.direction.up = 0;
            break;
          case SDLK_LEFT:
            game->player.direction.left = 0;

          case SDLK_RIGHT:
            game->player.direction.right = 0;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
}

