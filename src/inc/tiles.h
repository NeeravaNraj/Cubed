#ifndef TILES_H
#define TILES_H

#include "raylib.h"

typedef enum Tiles {
    Platform = 0,
    GrassPlatform,
    EndPoint,
    EndTile,

    SpawnPoint,
} Tiles;

typedef struct Tile {
    Tiles kind;
    Vector2 position;
} Tile;

void render_tile(Tile* tile, Vector2 offset);
void draw_tile(Tiles kind, Vector2 position, Vector2 offset);
#endif // !TILES_H
