#ifndef R_RENDER_H
#define R_RENDER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// handles the window
void RenderWindow(const char* p_title, int p_w, int p_h);

// load image
SDL_Texture* loadTexture(const char* p_filePath);
// drawing
void r_clear();
void r_render(SDL_Texture* p_tex);
void r_display();
// dealloc memory
void CloseWindow(void);
#endif
