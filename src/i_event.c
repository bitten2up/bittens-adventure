#include <SDL2/SDL.h>
#include <stdbool.h>

#include "i_event.h"
#include "g_game.h"
#include "e_collision.h"

struct direction {
  char x;
  char y;
};
void move(g_game* game);
SDL_Event event;
void i_poll(g_game* game)
{
  while (SDL_PollEvent(&event))
  {
    switch (event.type) {
      case SDL_QUIT:
        game->gameRunning=false;
        break;
      case SDL_KEYDOWN:
        switch( event.key.keysym.sym ) {
          case SDLK_ESCAPE:
            game->gameRunning=false;
            break;
          case SDLK_UP:
            if (game->state==overworld)
            {
              game->player.entity.src.x=0;
              game->player.direction.y=1;
            }
            break;
          case SDLK_DOWN:
            if (game->state==overworld){
              game->player.direction.y=-1;
              game->player.entity.src.x=32;
            }
            break;
          case SDLK_LEFT:
            if (game->state==overworld){
              game->player.direction.x=1;
              game->player.entity.src.x=64;
            }
            break;
          case SDLK_RIGHT:
            if (game->state==overworld){
              game->player.direction.x=-1;
              game->player.entity.src.x=96;
            }
            break;
          case SDLK_RETURN:
            if (game->state==title)
              game->state=overworld;
            break;
          default:
            break;
        }
        break;
      case SDL_KEYUP:
        switch( event.key.keysym.sym ) {
          case SDLK_DOWN:
          case SDLK_UP:
            game->player.direction.y=0;
            break;
          case SDLK_LEFT:
            if (game->player.direction.x > 0)
              game->player.direction.x = 0;
            break;

          case SDLK_RIGHT:
            if (game->player.direction.x < 0)
              game->player.direction.x = 0;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
  move(game);
}
void move(g_game* game)
{
  game->player.y+=game->player.direction.y;
  game->player.x+=game->player.direction.x;
  if (checkCollision(game->map, (game->map->width/2)-((game->player.x)/32)-5, (game->map->height/2)-((game->player.y+8)/32)-5) == CHESTS_LAYER)
  {
    game->player.y-=game->player.direction.y;
    game->player.x-=game->player.direction.x;
  }
}
