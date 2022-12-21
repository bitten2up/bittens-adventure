////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
// STD and Raylib
////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <raylib.h>
#include "tmx.h"


////////////////////////////////////////////////////////////
// ENGINE HEADERS
////////////////////////////////////////////////////////////

#include "bittendef.h"           // defines for the engine
#include "bit_collision.h"       // colision handling

int32_t checkCollision(tmx_map* map, int x, int y)
{
    // setup layers
    tmx_layer* chests = tmx_find_layer_by_id(map, CHESTS_LAYER); // chests
    //TraceLog(LOG_INFO, "value of tile: %i", chests->content.gids[y * map->width + x]);
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
        while (test!=NULL)
        {
            // make this work with tilex and tiley
            int tilex = ((test->x)/32);
            int tiley = ((test->y+8)/32)-1; // dont ask me wtf this has to be subtracted by 1 idk
            if ((y==tiley) && (x==tilex))
            {
                TraceLog(LOG_INFO, "GOTTEM");
                found=true;
                break;
            }
            else
            {
                //TraceLog(LOG_INFO, "tiley didn't work, y: %i, tiley: %i", y, tiley);
            }

            // go to next object
            test=test->next;
        }

        if (found){
            return test->content.gid;
        }
        return 0;

    }
    // simple layers are pritty easy
    else if (chests->type==L_LAYER)
    {
        return chests->content.gids[y * map->width + x];
    }

}
int disableCollision(tmx_map* map, int x, int y)
{
    return 0;
}