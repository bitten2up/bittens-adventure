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
// e_collision.c
/***********************************************************
* this file handles the collision between sprites
************************************************************/
// needs to be compleatly nuked most likely

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
#include "r_render.h"
#include "tmx.h"


////////////////////////////////////////////////////////////
// ENGINE HEADERS
////////////////////////////////////////////////////////////

#include "sdl_bittendef.h"           // defines for the engine
#include "e_collision.h"             // collision handling
#include "p_player.h"
#include "bit_game.h"
#include "discord.h"

///////
// quick tilepos to pos conversion function
///////
void undoTile(int tilex, int tiley);

int32_t checkCollision(tmx_map* map, int x, int y)
{
  // setup layers
  tmx_layer* chests = tmx_find_layer_by_name(map, "chests"); // chests
  //if (tile->user_data.integer){
  //    return 0;
  //}
  //TraceLog(LOG_INFO, "test id: %u", test->);
  // objects
  if (chests->type==L_OBJGR)
    {
      tmx_object* currentObject = chests->content.objgr->head;
      // go through the entire list of possible objects
      int counter = 0;
      int tilex = ((currentObject->x)/32);
      int tiley = ((currentObject->y)/32);
      currentObject = currentObject->next;
      if ((x == tilex) && (y == tiley))
      {
        return CHESTS_LAYER;
      }
      while (currentObject != NULL)
      {
        // make this work with tilex and tiley
        if ((x == tilex) && (y == tiley))
        {
          return CHESTS_LAYER;
        }
#ifdef DEBUG
        else
        {
          printf("tilex miss, x: %i, tilex: %i\n", x, tilex);
          printf("tiley miss, y: %i, tiley: %i\n", y, tiley);
        }
#endif
        counter++;
        // go to next object
        currentObject = currentObject->next;
      }

      return 0;

    }
    // simple layers are pretty easy
    // WHY THE FUCK DOES THIS NOT ALWAYS WORK - bitten 2024
  else if (chests->type == L_LAYER)
  {
  #ifdef DEBUG
    char buffer[2042];
    sprintf(buffer, "(%i %i)(%i,%i): %i", x, y, bitgame.player.x, bitgame.player.y, chests->content.gids[((y - 2) * map->width) + (x - 2)]);
    r_textbox(buffer, 400, 20);
  #endif

    return chests->content.gids[((y - 2) * map->width) + (x - 2)];
  }
  else
  {
    return 0;
  }
}

int32_t disableCollision(tmx_map* map, int x, int y)
{
  // setup layers
  tmx_layer* chests = tmx_find_layer_by_id(map, CHESTS_LAYER); // chests
  if (chests->type == L_OBJGR)
    {
      tmx_object* test = chests->content.objgr->head;
      // go through the entire list of possible objects
      bool found = false;
      int counter;
      while (test != NULL)
      {
        // make this work with tilex and tiley dont fucking question it
        int tilex = ((test->x)/32)+counter;
        int tiley = ((test->y+8)/32)-1; // dont ask me wtf this has to be subtracted by 1 idk
        if ((y == tiley) && (x == tilex))
        {
          found = true;
          break;
        }
        // go to next object
        test=test->next;
        if (counter == 0 )
          counter++;
      }

      if (found){
        test->content.gid = 3;
        return 0;
      }
      return 1;

    }
    // not fucking happening
  else
  {
    return 1;
  }
}


void undoTile(int tilex, int tiley) {
  bitgame.player.x = -(32*(tilex - (bitgame.map->width/2) + 3));
  bitgame.player.y = -(32*(tiley - (bitgame.map->height/2) + 3) + 14);
}
