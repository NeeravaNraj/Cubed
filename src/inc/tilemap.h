#ifndef TILEMAP_H
#define TILEMAP_H

#include "common.h"
#include "hashmap.h"
#include "tiles.h"
#include "arena.h"

typedef struct Tilemap {
    HashMap map;
    Tile** around;

    void (*render)();
} Tilemap;
void tilemap_init(Tilemap* tm);
void tilemap_add_tile(Tilemap* tm, Tile* tile);
Tile** tilemap_tiles_around(Tilemap* tm, Vector2 position);
void tilemap_render(Tilemap* tm);
#endif /* ifndef TILEMAP_H */
