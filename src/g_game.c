#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "g_game.h"
#include "sdl_bittendef.h"
#include "r_render.h"
#include "f_save.h"

g_game* g_init(void)
{
  g_game *game = malloc(sizeof(g_game));
  loadGame(game);
  game->state = title;
  // load sprite
  game->player.entitySprite.sprite = loadTexture("./assets/bitten.png");
  game->player.entitySprite.src.x = 0;
  game->player.entitySprite.src.y = 0;
  game->player.entitySprite.src.w = 32;
  game->player.entitySprite.src.h = 32;

  game->player.entitySprite.dst.x = SCREENWIDTH/2;
  game->player.entitySprite.dst.y = SCREENHEIGHT/2;
  game->player.entitySprite.dst.w = 32;
  game->player.entitySprite.dst.h = 32;

  tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;
  game->map = tmx_load("./assets/maps/bit_towntest.tmx");
  if (!game->map) {
    tmx_perror("Cannot load map");
    exit(1);
  }
  game->gameRunning = true;
  return game;
}
