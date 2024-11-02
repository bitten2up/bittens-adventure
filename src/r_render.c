/*
* MIT License
*
* Copyright (c) 2021-2024 bitten2up
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <tmx.h>
#include <cLDtk.h>

#include "sdl_bittendef.h"
#include "r_render.h"
#include "e_entity.h"
#include "bit_game.h"

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
SDL_Texture* text;
SDL_Rect textRec;

void* SDL_tex_loader(const char *path);

void InitWindow(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window failed to init %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer failed to init %s\n", SDL_GetError());
	}
	TTF_Init();
	font = TTF_OpenFont("assets/PressStart2P-Regular.ttf", 24);
	if (font == NULL) {
		printf("font failed to init %s\n", SDL_GetError());
  }
	tmx_img_load_func = SDL_tex_loader;
}

SDL_Texture* loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);
	if (texture == NULL)
	{
		printf("failed to load texture %s, SDL backtrace %s\n", p_filePath, SDL_GetError());
		exit(1);
	}
	return texture;
}



/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void r_text(char* message, int x, int y) {
  int text_width;
  int text_height;
  SDL_Surface *surface;
  SDL_Color textColor = {255, 255, 255, 0};

  surface = TTF_RenderText_Solid(font, message, textColor);
  text = SDL_CreateTextureFromSurface(renderer, surface);
  text_width = surface->w;
  text_height = surface->h;
  SDL_FreeSurface(surface);
  textRec.x = x - text_width * 0.5;
  textRec.y = y - text_height * 0.5;
  textRec.w = text_width;
  textRec.h = text_height;
  SDL_RenderCopy(renderer, text, NULL, &textRec);
	SDL_DestroyTexture(text);
}

// really bad textbox
void r_textbox(char* message, int x, int y) {
  int text_width;
  int text_height;
  SDL_Surface *surface;
  SDL_Color textColor = {0, 0, 0, 0};

  surface = TTF_RenderText_Solid(font, message, textColor);
  text_width = surface->w;
  text_height = surface->h;
	r_rect(x,y,text_width+10,text_height+10);
  text = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  textRec.x = x - text_width * 0.5;
  textRec.y = y - text_height * 0.5;
  textRec.w = text_width;
  textRec.h = text_height;
  SDL_RenderCopy(renderer, text, NULL, &textRec);
	SDL_DestroyTexture(text);
}

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
// shit probally should have a struct but who fucking cares
void r_rect(int x, int y, int w, int h) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
  textRec.x = x - w * 0.5;
  textRec.y = y - h * 0.5;
  textRec.w = w;
  textRec.h = h;
  SDL_RenderFillRect(renderer, &textRec);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_DestroyTexture(text);
}

void CloseWindow(void)
{
	SDL_DestroyTexture(text);
  TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void r_clear(void)
{
	SDL_RenderClear(renderer);
}

void r_sprite(e_entitySprite* e)
{
	SDL_RenderCopy(renderer, e->sprite, &e->src, &e->dst);
}


void r_display()
{
	SDL_RenderPresent(renderer);
}

//////////////
// cLDtk shit
//////////////
static void DrawSprite(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect sourceRect, SDL_Rect destinationRect, int flip)
{
    SDL_RendererFlip sdl_flip = SDL_FLIP_NONE;

    switch (flip)
    {
    case 0:
        sdl_flip = SDL_FLIP_NONE;
        break;
    case 1:
        sdl_flip = SDL_FLIP_HORIZONTAL;
        break;
    case 2:
        sdl_flip = SDL_FLIP_VERTICAL;
        break;
    case 3:
        sdl_flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
        break;
    }

    SDL_RenderCopyEx(renderer, texture, &sourceRect, &destinationRect, 0, NULL, sdl_flip);
}

void r_drawTiles(struct layerInstances* layer, SDL_Renderer* renderer, SDL_Texture* texture)
{
    for (int y = layer->autoTiles_data_ptr->count; y-- > 0;)
    {
        DrawSprite(renderer, texture,
            (SDL_Rect){ layer->autoTiles_data_ptr[y].SRCx, layer->autoTiles_data_ptr[y].SRCy, bitgame.player.y, bitgame.player.x },
            (SDL_Rect){ layer->autoTiles_data_ptr[y].x, layer->autoTiles_data_ptr[y].y, bitgame.player.y, bitgame.player.x },
            layer->autoTiles_data_ptr[y].f);
    }
}

//////////////
// libtmx shit
//////////////

void* SDL_tex_loader(const char *path) {
	return IMG_LoadTexture(renderer, path);
}

void set_color(int color) {
	tmx_col_bytes col = tmx_col_to_bytes(color);
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
}

void draw_polyline(double **points, double x, double y, int pointsc) {
	int i;
	for (i=1; i<pointsc; i++) {
		SDL_RenderDrawLine(renderer, x+points[i-1][0], y+points[i-1][1], x+points[i][0], y+points[i][1]);
	}
}

void draw_polygon(double **points, double x, double y, int pointsc) {
	draw_polyline(points, x, y, pointsc);
	if (pointsc > 2) {
		SDL_RenderDrawLine(renderer, x+points[0][0], y+points[0][1], x+points[pointsc-1][0], y+points[pointsc-1][1]);
	}
}

void draw_objects(tmx_object_group *objgr) {
	SDL_Rect rect;
	set_color(objgr->color);
	tmx_object *head = objgr->head;
	while (head) {
		if (head->visible) {
			if (head->obj_type == OT_SQUARE) {
				rect.x = head->x;
        rect.y = head->y;
				rect.w = head->width;
        rect.h = head->height;
				SDL_RenderDrawRect(renderer, &rect);
			}
			else if (head->obj_type  == OT_POLYGON) {
				draw_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
			}
			else if (head->obj_type == OT_POLYLINE) {
				draw_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
			}
			else if (head->obj_type == OT_ELLIPSE) {
				/* FIXME: no function in SDL2 */
			}
		}
		head = head->next;
	}
}

void draw_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags) {
	SDL_Rect src_rect, dest_rect;
	src_rect.x = sx;
	src_rect.y = sy;
	src_rect.w = dest_rect.w = sw;
	src_rect.h = dest_rect.h = sh;
	dest_rect.x = dx;
	dest_rect.y = dy;
	SDL_RenderCopy(renderer, (SDL_Texture*)image, &src_rect, &dest_rect);
}

void draw_layer(tmx_map *map, tmx_layer *layer) {
	unsigned long i, j;
	unsigned int gid, x, y, w, h, flags;
	float op;
	tmx_tileset *ts;
	tmx_image *im;
	void* image;
	op = layer->opacity;
	for (i=0; i<map->height; i++) {
		for (j=0; j<map->width; j++) {
			gid = (layer->content.gids[(i*map->width)+j]) & TMX_FLIP_BITS_REMOVAL;
			if (map->tiles[gid] != NULL) {
				ts = map->tiles[gid]->tileset;
				im = map->tiles[gid]->image;
				x  = map->tiles[gid]->ul_x;
				y  = map->tiles[gid]->ul_y;
				w  = ts->tile_width;
				h  = ts->tile_height;
				if (im) {
					image = im->resource_image;
				}
				else {
					image = ts->image->resource_image;
				}
				flags = (layer->content.gids[(i*map->width)+j]) & ~TMX_FLIP_BITS_REMOVAL;
				draw_tile(image, x, y, w, h, ((j+bitgame.player.x)*ts->tile_width), ((i+bitgame.player.y)*ts->tile_height), op, flags);
			}
		}
	}
}

void draw_image_layer(tmx_image *image) {
	SDL_Rect dim;
	dim.x = 0;
	dim.y = 0;

	SDL_Texture *texture = (SDL_Texture*)image->resource_image; // Texture loaded by libTMX
	SDL_QueryTexture(texture, NULL, NULL, &(dim.w), &(dim.h));
	SDL_RenderCopy(renderer, texture, NULL, &dim);
	SDL_DestroyTexture(texture);
}

void draw_all_layers(tmx_map *map, tmx_layer *layers) {
	while (layers) {
		if (layers->visible) {

			if (layers->type == L_GROUP) {
				draw_all_layers(map, layers->content.group_head);
			}
			else if (layers->type == L_OBJGR) {
				draw_objects(layers->content.objgr);
			}
			else if (layers->type == L_IMAGE) {
				draw_image_layer(layers->content.image);
			}
			else if (layers->type == L_LAYER) {
				draw_layer(map, layers);
			}
		}
		layers = layers->next;
	}
}

void render_map(tmx_map *map) {
	set_color(map->backgroundcolor);
	draw_all_layers(map, map->ly_head);
}
