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
#endif /* ifndef TILEMAP_H */
