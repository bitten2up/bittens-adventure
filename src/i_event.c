#include <SDL2/SDL.h>
#include <stdbool.h>

#include "i_event.h"
bool quit=false;

SDL_Event event;
void i_poll(bool* gameRunning)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
		case SDL_QUIT:
			*gameRunning=false;
			break;
		case SDL_KEYDOWN:
			switch( event.key.keysym.sym ) {
				case SDLK_ESCAPE:
					*gameRunning=false;
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
