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
    return chests->content.gids[y * 30 + x];
}
