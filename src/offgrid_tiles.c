#include "inc/tiles.h"
#include "inc/vector.h"
#include "inc/common.h"
#include "inc/offgrid.h"
#include "inc/raylib/raylib.h"

void offgrid_init(OffgridTiles* off_tiles) {
    Vec_init(off_tiles->tiles, 8, NULL); // NOLINT
}

void offgrid_add_tile(OffgridTiles* off_tiles, Tile* tile) {
    int tile_x = tile->position.x / TILE_SIZE;
    int tile_y = tile->position.y / TILE_SIZE;

    tile->position.x = tile_x * TILE_SIZE;
    tile->position.y = tile_y * TILE_SIZE;

    Vec_push(off_tiles->tiles, tile); // NOLINT
}


void offgrid_remove_tile(OffgridTiles* off_tiles, Vector2 position) {
    int tile_x = position.x / TILE_SIZE;
    int tile_y = position.y / TILE_SIZE;

    position.x = tile_x * TILE_SIZE;
    position.y = tile_y * TILE_SIZE;

    for (int i = 0; i < Vec_length(off_tiles->tiles); ++i) {
        Tile* tile = off_tiles->tiles[i];
        if (tile->position.x == position.x && tile->position.y == position.y) {
            Vec_delete(off_tiles->tiles, i); // NOLINT
            break;
        }
    }
}


void offgrid_render(OffgridTiles* off_tiles, Vector2 offset) {
    for (int i = 0; i < Vec_length(off_tiles->tiles); ++i) {
        Tile* tile = off_tiles->tiles[i];
        render_tile(tile, offset);
    }
}

void offgrid_deinit(OffgridTiles* off_tiles) {
    Vec_free(off_tiles->tiles);
    off_tiles->tiles = NULL;
}
