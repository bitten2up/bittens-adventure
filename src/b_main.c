#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

// bitten headers
#include "sdl_bittendef.h"
#include "r_render.h"
#include "i_event.h"
#include "e_entity.h"

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
	e_entity player;
	player.sprite = loadTexture("../assets/bitten.png");
	player.src.x = 0;
	player.src.y = 0;
	player.src.w = 32;
	player.src.h = 32;

	player.dst.x = 0;
	player.dst.y = 0;
	player.dst.w = 32;
	player.dst.h = 32;
	
	// keep on running game while it is open
	
	bool gameRunning = true;

	while (gameRunning)
	{
		i_poll(&gameRunning);
		r_clear();
		player.src.x=64;
		r_renderer(&player);
		r_display();
	}
	CloseWindow();
	SDL_Quit();
	return 0;
}
