#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "bit_game.h"
#include "sdl_bittendef.h"
#include "r_render.h"
#include "f_save.h"

bit_game bitgame;
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

  tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;
  bitgame.map = tmx_load("./assets/maps/bit_towntest.tmx");
  if (!bitgame.map) {
    tmx_perror("Cannot load map");
    exit(1);
  }
  bitgame.gameRunning = true;
  
  bit_main();

  return 0;
}
