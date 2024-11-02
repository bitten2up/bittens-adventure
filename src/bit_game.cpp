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

extern "C"
{
  #include "sdl_bittendef.h"
  #include "bit_game.h"
  #include "r_render.h"
  #include "i_event.h"
  #include "e_entity.h"
  #include "bit_game.h"
  #include "p_player.h"
  #include "b_battle.h"
  #include "e_collision.h"
  #include "f_save.h"
}

void bit_main()
{
  bitgame.preformance.startTime = 0;
  bitgame.preformance.endTime = 0;
  bitgame.preformance.deltaTime = 0;
  bitgame.preformance.fps = 60;
  bitgame.preformance.timePerFrame = 16; // miliseconds
  bitgame.preformance.showFps = true;


  while (bitgame.gameRunning)
  {
    i_poll();
    r_clear();
    switch (bitgame.state){
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
        render_map(bitgame.map);
        r_sprite(&bitgame.player.entitySprite);
        p_moveOverworld();
        break;
      case platformer:
        #ifdef DISCORD
        updateDiscordPresence("platforming", "wow");
        #endif
        p_movePlatformer();
        render_map(bitgame.map);
        r_sprite(&bitgame.player.entitySprite);
        break;
      case battle:
        #ifdef DISCORD
        updateDiscordPresence("Battling", "herself");
        #endif
        b_battle();
        break;
      default:
        #ifdef DISCORD
        updateDiscordPresence("wat", "this dont make sense. \%_\%");
        #endif
        break;
    }

    r_display();

    if (!bitgame.preformance.startTime) {
      // get the time in ms passed from the moment the program started
      bitgame.preformance.startTime = SDL_GetTicks();
    }
    else {
      bitgame.preformance.deltaTime = bitgame.preformance.endTime - bitgame.preformance.startTime; // how many ms for a frame
    }


    // if less than 16ms, delay 
    if (bitgame.preformance.deltaTime < bitgame.preformance.timePerFrame) {
      SDL_Delay(bitgame.preformance.timePerFrame - bitgame.preformance.deltaTime);
    }

    // if deltaTime is bigger than 16ms between frames, get the actual fps
    if (bitgame.preformance.deltaTime > bitgame.preformance.timePerFrame) {
      bitgame.preformance.fps = 1000 / bitgame.preformance.deltaTime;
    }
    

    bitgame.preformance.startTime = bitgame.preformance.endTime;
    bitgame.preformance.endTime = SDL_GetTicks();
  }
  saveGame();
  tmx_map_free(bitgame.map);
  CloseWindow();
  SDL_Quit();
  return;
}
