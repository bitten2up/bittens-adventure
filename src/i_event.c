#include <SDL2/SDL.h>
#include <stdbool.h>

#include "i_event.h"
#include "g_game.h"

bool quit=false;

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
						game->player.y+=2;
					break;
				case SDLK_DOWN:
					if (game->state==overworld)
						game->player.y-=2;
					break;
				case SDLK_LEFT:
					if (game->state==overworld)
						game->player.x+=2;
					break;
				case SDLK_RIGHT:
					if (game->state==overworld)
						game->player.x-=2;
					break;
				case SDLK_RETURN:
					if (game->state==title)
						game->state=overworld;
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
