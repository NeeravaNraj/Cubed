#ifndef OFFGRID_H
#define OFFGRID_H

#include "raylib.h"
#include "tiles.h"
#include "vector.h"

typedef struct {
    Vec(Tile*) tiles;
} OffgridTiles;

void offgrid_init(OffgridTiles* off_tiles);
void offgrid_add_tile(OffgridTiles* off_tiles, Tile* tile);
void offgrid_render(OffgridTiles* off_tiles, Vector2 offset);
void offgrid_remove_tile(OffgridTiles* off_tiles, Vector2 position);
void offgrid_deinit(OffgridTiles* off_tiles);
#endif // !OFFGRID_H
