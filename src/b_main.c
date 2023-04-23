#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "sdl_bittendef.h"
#include "r_render.h"

int main (int argc, char argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO > 0))
	{
		printf("ERROR: SDL has failed to init %c\n", SDL_GetError());
	}
	if (!(IMG_Init(IMG_INIT_PNG)))
	{
		printf("ERROR: SDL_image has failed to init png %c\n", SDL_GetError());
	}
	RenderWindow(GAME_NAME, SCREENWIDTH, SCREENHEIGHT);
	
	// load sprite
	SDL_Texture* player = loadTexture("../assets/bitten.png");
	// keep on running game while it is open
	
	bool gameRunning = true;
	// take input
	SDL_Event event;

	while (gameRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type){
				case SDL_QUIT:
					gameRunning = false;
					break;
				case SDL_KEYDOWN:
					gameRunning = false;
					break;
				default:
					break;
			}
		}
		r_clear();
		r_renderer(player);
		r_display();
	}
	CloseWindow();
	SDL_Quit();
	return 0;
}
