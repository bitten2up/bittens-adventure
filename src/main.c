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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "discord.h"

//////////////////
// bitten headers
//////////////////

#include "sdl_bittendef.h"
#include "r_render.h"
#include "i_event.h"
#include "e_entity.h"
#include "g_game.h"

int main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO > 0))
  {
    printf("ERROR: SDL has failed to init %s\n", SDL_GetError());
  }
  if (!(IMG_Init(IMG_INIT_PNG)))
  {
    printf("ERROR: SDL_image has failed to init png %s\n", SDL_GetError());
  }

#ifdef DISCORD
  discordInit();
#endif

  InitWindow(GAME_NAME, SCREENWIDTH, SCREENHEIGHT);
  g_game game;
  game.state = title;
  // load sprite
  game.player.entity.sprite = loadTexture("./assets/bitten.png");
  game.player.entity.src.x = 0;
  game.player.entity.src.y = 0;
  game.player.entity.src.w = 32;
  game.player.entity.src.h = 32;

  game.player.entity.dst.x = SCREENWIDTH/2;
  game.player.entity.dst.y = SCREENHEIGHT/2;
  game.player.entity.dst.w = 32;
  game.player.entity.dst.h = 32;

  tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;
  game.map = tmx_load("./assets/maps/bit_towntest.tmx");
  if (!game.map) {
    tmx_perror("Cannot load map");
    return 1;
  }
  Uint32 startTime = 0;
  Uint32 endTime = 0;
  Uint32 delta = 0;
  short fps = 60;
  short timePerFrame = 16; // miliseconds
  game.gameRunning = true;

  while (game.gameRunning)
  {
    i_poll(&game);
    r_clear();
    switch (game.state){
      case title:
        #ifdef DISCORD
        updateDiscordPresence("title screen", "press start");
        #endif
        r_text("bitten's adventure", SCREENWIDTH/2, SCREENHEIGHT/2);
        break;
      case overworld:
        #ifdef DISCORD
        updateDiscordPresence("Overworld", "e");
        #endif
         break;
      case battle:
        #ifdef DISCORD
        updateDiscordPresence("","");
        #endif
        break;
      default:
        #ifdef DISCORD
        updateDiscordPresence("wat", "this dont make sense. %_%");
        #endif
        break;
    }
    

    if (game.state == overworld){
      render_map(game.map, &game);
      r_sprite(&game.player.entity);
    }
    r_display();
    if (!startTime) {
      // get the time in ms passed from the moment the program started
      startTime = SDL_GetTicks(); 
      } else {
      delta = endTime - startTime; // how many ms for a frame
    }


    // if less than 16ms, delay 
    if (delta < timePerFrame) {
      SDL_Delay(timePerFrame - delta);
    }

    // if delta is bigger than 16ms between frames, get the actual fps
    if (delta > timePerFrame) {
      fps = 1000 / delta;
    }
    

    startTime = endTime;
    endTime = SDL_GetTicks();
  }
  tmx_map_free(game.map);
  CloseWindow();
  SDL_Quit();
  return 0;
}
