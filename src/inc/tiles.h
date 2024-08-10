#ifndef TILES_H
#define TILES_H
#include <raylib.h>

typedef enum Tiles {
    Platform,
    GrassPlatform,
} Tiles;

typedef struct Tile {
    Tiles kind;
    Vector2 position;
} Tile;

void render_tile(Tile* tile);
#endif // !TILES_H
