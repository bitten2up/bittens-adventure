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
//----------------------------------------------------------
// r_render.c
/***********************************************************
* this file handles all rendering related logic
************************************************************/
// very dumb and we have to work around some of the things it does, but it works

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
// STD, SDL, and tmx
////////////////////////////////////////////////////////////

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#if defined(BITVULKAN)
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#elif defined(BITGLES2)
#include <glad/glad.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>
#endif
#include <tmx.h>
//#include <cLDtk.h> // sorry cLDtk, tmx seems to be working fine for now :)

////////////////////////////////////////////////////////////
// ENGINE HEADERS
////////////////////////////////////////////////////////////

#include "sdl_bittendef.h"           // defines for the engine
#include "r_render.h"
#include "e_entity.h"
#include "bit_game.h"

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
SDL_Texture* text;
SDL_Rect textRec;

#if defined(BITVULKAN)
// vulkan shit
VkInstance vkInst;
VkPhysicalDevice *physical_devices;
VkPhysicalDevice gpu;
VkDevice device;
VkQueueFamilyProperties *queue_props;
VkPhysicalDeviceFeatures features;

uint32_t physicalDeviceCount;
uint32_t extension_count;
uint32_t queue_count;
const char** extension_names = 0;

const VkApplicationInfo app = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = NULL,
    .pApplicationName = "BITTENS_ADVENTURE",
    .applicationVersion = 0,
    .pEngineName = "BITTEN ENGINE",
    .engineVersion = 0,
    .apiVersion = VK_API_VERSION_1_0,
};
VkInstanceCreateInfo inst_info = {
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pNext = NULL,
	.pApplicationInfo = &app,
	.enabledLayerCount = NULL,
	.ppEnabledLayerNames = NULL,
	.enabledExtensionCount = extension_count,
	.ppEnabledExtensionNames = (const char *const *) extension_names;
};

float queue_priorities[1] = {0.0};
uint32_t graphicsQueueIndex = UINT32_MAX;

const VkDeviceQueueCreateInfo queue = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext = NULL,
    .queueFamilyIndex = graphicsQueueIndex,
    .queueCount = 1,
    .pQueuePriorities = queue_priorities
};

void InitVulkan()
{
	// vulkan shit
	SDL_Vulkan_GetInstanceExtensions(window, &extension_count, extension_names);

	vkCreateInstance(&inst_info, NULL, vkInst);

	physical_devices = malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);
	VkResult err = vkEnumeratePhysicalDevices(vkInst, physicalDeviceCount);
	gpu = physical_devices[0];

	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_count, queue_props);
	assert(queue_count >= 1);

	vkGetPhysicalDeviceFeatures(gpu, &features);

	uint32_t graphicsQueueIndex = UINT32_MAX;
	for (i = 0; i < queue_count; i++) {
	    if ((queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
	        if (graphicsQueueIndex == UINT32_MAX)
	            graphicsQueueIndex = i;
	   }
	}

	VkDeviceCreateInfo createInfo = {
	    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	    .pNext = NULL,
	    .queueCreateInfoCount = 1,
	    .pQueueCreateInfos = &queue,
	    .enabledLayerCount = 0,
	    .ppEnabledLayerNames = NULL,
	    .enabledExtensionCount = extension_count,
	    .ppEnabledExtensionNames = (const char *const *)extension_names,
	    .pEnabledFeatures = NULL
	};

	vkCreateDevice(gpu, &device, NULL, createInfo);
}
#elif defined(BITGLES2)
const GLchar* vertexSource =
	"attribute vec4 position;\n"
	"void main()\n"
	"{\n"
	"	gl_position = vec4(position, 1.0);\n"
	"}\n";

const GLchar* fragmentSource =
	"percision mediump float;\n"
	"void main()\n"
	"{\n"
	"	gl_Fragcolor = vec4(10.,1.0,1.0,1.0);\n"
	"}\n";

SDL_GLContext glContext;
SDL_Texture* shaderOverlay;
GLuint vao, vbo;
GLuint shaderProgram;
GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f};
void InitGles(void)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	glContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, glContext);
	gladLoadGLES2Loader(SDL_GL_GetProcAddress);
	SDL_GL_SetSwapInterval(0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	// triangle

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	shaderOverlay = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREENWIDTH, SCREENHEIGHT);
}
#endif

void* SDL_tex_loader(const char *path);

void InitWindow(const char* p_title, int p_w, int p_h)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		printf("Window failed to init %s\n", SDL_GetError());
	}


	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer failed to init %s\n", SDL_GetError());
	}

#if defined(BITVULKAN)
	InitVulkan();
#elif defined(BITGLES2)
	InitGles();
#endif

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

void freeTexture(SDL_Texture* t)
{
	if (t == NULL)
	{
		printf("double free on texture...");
		exit(1);
	}

	SDL_DestroyTexture(t);

	t = NULL; // for my sanity
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
#if defined(BITVULKAN)
void VulkanShutdown(void)
{
	vkDestroyDevice(device, NULL);
	vkDestroyInstance(vkInst, NULL);
}
#elif defined(BITGLES2)
void GlesShutdown(void)
{
	SDL_GL_DeleteContext(glContext);
}
#endif

void CloseWindow(void)
{
	SDL_DestroyTexture(text);
  TTF_Quit();
	SDL_DestroyRenderer(renderer);
#if defined(BITVULKAN)
	VulkanShutdown();
#elif defined(BITGLES2)
	GlesShutdown();
#endif
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void r_clear(void)
{
#ifdef BITGLES2
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//SDL_SetRenderTarget(renderer, shaderOverlay);
#endif

	SDL_RenderSetViewport(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void r_sprite(e_sprite* e)
{
	SDL_RenderCopy(renderer, e->sprite, &e->src, &e->dst);
}


#ifdef BITGLES2
static void DisplayGles(void)
{
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderSetViewport(renderer, NULL);

	SDL_RenderFlush(renderer);
	SDL_GL_BindTexture(shaderOverlay, NULL, NULL);
	glDrawArrays(GL_TRIANGLES,0,3);
}
#endif

void r_display()
{
	SDL_RenderPresent(renderer);
#ifdef BITGLES2
	DisplayGles();
#endif
}

//////////////
// cLDtk shit
//////////////
#if 0 // maybe later
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
#endif

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
				draw_tile(image, x, y, w, h, ((j+bitgame.player.x/32)*ts->tile_width+bitgame.player.x%32), ((i+bitgame.player.y/32)*ts->tile_height+bitgame.player.y%32), op, flags);
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
