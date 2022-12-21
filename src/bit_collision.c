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
        while (test!=NULL)
        {
            test->visible=0;
            test=test->next;
        }

    }
    // simple layers are pritty easy
    else if (chests->type==L_LAYER)
    {
        return chests->content.gids[y * map->width + x];
    }

}
int disableCollision(tmx_map* map, int x, int y)
{
    tmx_tile* tile=map->tiles[y * map->width + x];
    tile->user_data.integer=1;
    return 0;
}