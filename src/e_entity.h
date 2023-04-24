#ifndef E_ENTITY_H
#define E_ENTITY_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
typedef struct e_entity e_entity;
typedef struct e_player e_player;

struct e_entity {
	SDL_Texture* sprite;
	SDL_Rect src;
	SDL_Rect dst;
};

struct e_player{
	e_entity entity;
	char name[10];
	int x;
	int y;
};
#endif
