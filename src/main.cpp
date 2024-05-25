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

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cLDtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern "C"
{
#include "bit_game.h"
#include "sdl_bittendef.h"
#include "r_render.h"
#include "f_save.h"
}

bit_game bitgame;
int main(int argc, char* argv[])
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  if (SDL_Init(SDL_INIT_VIDEO > 0))
  {
    printf("ERROR: SDL has failed to init %s\n", SDL_GetError());
    return 1;
  }
  if (!(IMG_Init(IMG_INIT_PNG)))
  {
    printf("ERROR: SDL_image has failed to init png %s\n", SDL_GetError());
    return 1;
  }
  InitWindow(GAME_NAME, SCREENWIDTH, SCREENHEIGHT);

#ifdef DISCORD
  discordInit();
#endif

  loadGame();
  bitgame.state = title;
  // load sprite
  bitgame.player.entitySprite.sprite = loadTexture("./assets/bitten.png");
  bitgame.player.entitySprite.src.x = 0;
  bitgame.player.entitySprite.src.y = 0;
  bitgame.player.entitySprite.src.w = 32;
  bitgame.player.entitySprite.src.h = 32;

  bitgame.player.entitySprite.dst.x = SCREENWIDTH/2;
  bitgame.player.entitySprite.dst.y = SCREENHEIGHT/2;
  bitgame.player.entitySprite.dst.w = 32;
  bitgame.player.entitySprite.dst.h = 32;
  bitgame.player.gravity = defaultGravity;
  bitgame.player.voly = 0;

  tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;
  bitgame.map = tmx_load("./assets/maps/bit_towntest.tmx");
  if (!bitgame.map) {
    tmx_perror("Cannot load map");
    exit(1);
  }
  //loadJSONFile("{\"jsonVersion\":\"\"}", "assets/test.ldtk");

  bitgame.gameRunning = true;
  
  bit_main();

  return 0;
}
