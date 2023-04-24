#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "r_render.h"
#include "e_entity.h"

SDL_Window* window;
SDL_Renderer* renderer;

void RenderWindow(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window failed to init %c\n", SDL_GetError());
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer failed to init %c\n", SDL_GetError());
	}
}

SDL_Texture* loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture=IMG_LoadTexture(renderer, p_filePath);
	if (texture == NULL)
		printf("failed to load texture %s, SDL backtrace %s\n", p_filePath, SDL_GetError());
	return texture;
}

void CloseWindow(void)
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void r_clear(void)
{
	SDL_RenderClear(renderer);
}

void r_renderer(e_entity* e)
{
	SDL_RenderCopy(renderer, e->sprite, &e->src, &e->dst);
}

void r_display()
{
	SDL_RenderPresent(renderer);
}
