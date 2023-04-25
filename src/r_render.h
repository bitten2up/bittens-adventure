#ifndef R_RENDER_H
#define R_RENDER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "e_entity.h"
#include "g_game.h"

// handles the window
void RenderWindow(const char* p_title, int p_w, int p_h);

// load image
SDL_Texture* loadTexture(const char* p_filePath);
// drawing
void r_clear();
void r_renderer(e_entity* e);
void r_display();
// libtmx shit
void render_map(tmx_map *map, g_game* game);
// dealloc memory
void CloseWindow(void);
#endif
