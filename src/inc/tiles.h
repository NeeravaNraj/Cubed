#ifndef TILES_H
#define TILES_H
#include <raylib.h>

typedef enum Tiles {
    Platform = 0,
    GrassPlatform,
    EndTile,
} Tiles;

typedef struct Tile {
    Tiles kind;
    Vector2 position;
} Tile;

void render_tile(Tile* tile);
void draw_tile(Tiles kind, Vector2 position);
#endif // !TILES_H
