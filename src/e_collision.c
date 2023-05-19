////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
// STD and SDL
////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "tmx.h"


////////////////////////////////////////////////////////////
// ENGINE HEADERS
////////////////////////////////////////////////////////////

#include "sdl_bittendef.h"           // defines for the engine
#include "e_collision.h"       // colision handling
#include "g_game.h"
#include "discord.h"

///////
// quick tilepos to pos conversion function
///////
void undoTile(g_game *game, int tilex, int tiley);

int32_t checkCollision(tmx_map* map, int x, int y)
{
  // setup layers
  tmx_layer* chests = tmx_find_layer_by_id(map, CHESTS_LAYER); // chests
#ifdef DEBUGBUILD
  printf("value of tile: %i\n", chests->content.gids[y * map->width + x]);
#endif
  //if (tile->user_data.integer){
  //    return 0;
  //}
  //TraceLog(LOG_INFO, "test id: %u", test->);
  // objects
  if (chests->type==L_OBJGR)
    {
      tmx_object* test= chests->content.objgr->head;
      // go through the entire list of possible objects
      bool found=false;
      int counter=0;
      while (test!=NULL)
      {
        // make this work with tilex and tiley
        int tilex = ((test->x)/32);
        int tiley = ((test->y+8)/32)-1; // dont ask me wtf this has to be subtracted by 1 idk
        if ((x==tilex) && (y==tiley))
        {
          found=true;
          break;
        }
#ifdef DEBUGBUILD
        else
        {
          printf("tilex didn't work, x: %i, tilex: %i\n", x, tilex);
          printf("tiley didn't work, y: %i, tiley: %i\n", y, tiley);
        }
#endif
      counter++;
        // go to next object
        test=test->next;
      }

      if (found){
        return CHESTS_LAYER;
      }
      return 0;

    }
    // simple layers are pritty easy
  else if (chests->type==L_LAYER)
  {
    return chests->content.gids[y * map->width + x];
  }
}

int32_t disableCollision(tmx_map* map, int x, int y)
{
  // setup layers
  tmx_layer* chests = tmx_find_layer_by_id(map, CHESTS_LAYER); // chests
  if (chests->type==L_OBJGR)
    {
      tmx_object* test= chests->content.objgr->head;
      // go through the entire list of possible objects
      bool found=false;
      int counter;
      while (test!=NULL)
      {
        // make this work with tilex and tiley
        int tilex = ((test->x)/32)+counter;
        int tiley = ((test->y+8)/32)-1; // dont ask me wtf this has to be subtracted by 1 idk
        if ((y==tiley) && (x==tilex))
        {
          found=true;
          break;
        }
        // go to next object
        test=test->next;
        if (counter==0)     {counter++;}
      }

      if (found){
        test->content.gid=3;
        return 0;
      }
      return 1;

    }
    // simple layers are pritty easy
  else if (chests->type==L_LAYER)
  {
  return 1;
}
}


void undoTile(g_game* game, int tilex, int tiley) {
  game->player.x = -(32*(tilex - (game->map->width/2) + 3));
  game->player.y = -(32*(tiley - (game->map->height/2) + 3) + 14);
}
