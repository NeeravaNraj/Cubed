#ifndef TILES_H
#define TILES_H

#include <stdint.h>

#include "box2d/id.h"
#include "raylib/raylib.h"
#include "box2d/math_functions.h"

typedef enum TileType {
    OngridTile = 1,
    OffgridTile,
} TileType;

typedef enum Tiles {
    GrassPlatform = 0,
    StonePlatform,

    SmallDecor,

    TilesEnd
} Tiles;

void tile_create(Vector2 position, Tiles kind, char variant);
Vector2 resolve_tile_position(Vector2 pos);
#endif // !TILES_H
