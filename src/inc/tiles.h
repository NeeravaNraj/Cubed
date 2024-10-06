#ifndef TILES_H
#define TILES_H

#include "raylib.h"
#include <stdint.h>

typedef enum TileType {
    OngridTile = 0,
    OffgridTile,
} TileType;

typedef enum Tiles {
    GrassPlatform = 0,
    StonePlatform,
    SmallDecor,
    EndTile,

    EndPoint,
    SpawnPoint,
} Tiles;

typedef struct Tile {
    Tiles kind;
    char variant;
    Vector2 position;
} Tile;

void render_tile(Tile* tile, Vector2 offset);
void draw_tile(Tiles kind, char variant, Vector2 position, Vector2 offset);
#endif // !TILES_H
