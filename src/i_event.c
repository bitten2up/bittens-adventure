#include <SDL2/SDL.h>
#include <stdbool.h>

#include "i_event.h"


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
				keyboardIn();
				*gameRunning=false;
				break;
			default:
				break;
		}
	}
}
void keyboardIn()
{
	if (strcmp(")", SDL_GetKeyName(event.key.keysym.scancode)))
		printf("E\n");
}
