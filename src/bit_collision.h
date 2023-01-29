#ifndef BIT_COLLISION_H
#define BIT_COLLISION_H

#include "tmx.h"
#include "bittendef.h"
int32_t checkCollision(tmx_map* map, int x, int y);
int disableCollision(tmx_map* map, int x, int y);
void undoTile(bit_game *game, int tilex, int tiley);
#endif
