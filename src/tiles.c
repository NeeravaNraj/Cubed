#include "inc/tiles.h"
#include "inc/common.h"
#include <raylib.h>

#define PLATFORM_TOP_COLOR (0x642d0aff)
#define PLATFORM_COLOR (0xbf6950ff)

#define GRASS_TOP_COLOR (0x329114ff)

void render_grass_platform(Tile* tile) {
    int offset = 6;
    int height = 12;
    int top_height = 5;

    Vector2 bottom_pos = tile->position;
    bottom_pos.x += offset / 2; // NOLINT

    Vector2 top_size = (Vector2){.x = TILE_SIZE + offset, .y = top_height};
    Vector2 bottom_size = (Vector2){.x = TILE_SIZE, .y = height};

    DrawRectangleV(bottom_pos, bottom_size, GetColor(PLATFORM_COLOR));
    DrawRectangleV(tile->position, top_size, GetColor(GRASS_TOP_COLOR));
}

void render_platform(Tile* tile) {
    int offset = 6;
    int height = 12;
    int top_height = 5;

    Vector2 bottom_pos = tile->position;
    bottom_pos.x += offset / 2; // NOLINT

    Vector2 top_size = (Vector2){.x = TILE_SIZE + offset, .y = top_height};
    Vector2 bottom_size = (Vector2){.x = TILE_SIZE, .y = height};

    DrawRectangleV(bottom_pos, bottom_size, GetColor(PLATFORM_COLOR));
    DrawRectangleV(tile->position, top_size, GetColor(PLATFORM_TOP_COLOR));
}

void render_tile(Tile* tile) {

    switch (tile->kind) {
        case Platform:
            render_platform(tile);
            break;

        case GrassPlatform:
            render_grass_platform(tile);
            break;
    }
}
