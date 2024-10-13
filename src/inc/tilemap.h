#ifndef TILEMAP_H
#define TILEMAP_H

#include "common.h"
#include "hashmap.h"
#include "tiles.h"
#include "arena.h"

typedef struct Tilemap {
    HashMap map;
    Tile** around;
} Tilemap;

void tilemap_init(Tilemap* tm);
void tilemap_add_tile(Tilemap* tm, Tile* tile);
Tile** tilemap_tiles_around(Tilemap* tm, Vector2 position);
void tilemap_render(Tilemap* tm, Vector2 offset);
Tile* tilemap_get_tile(Tilemap* tm, Vector2 position);
void tilemap_auto_tile(Tilemap* tm);
bool tilemap_remove_tile(Tilemap* tm, Vector2 position);
#endif /* ifndef TILEMAP_H */
