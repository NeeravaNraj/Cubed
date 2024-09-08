#include "inc/tiles.h"
#include "inc/common.h"
#include <raylib.h>
#include <raymath.h>

#define PLATFORM_TOP_COLOR (0x642d0aff)
#define PLATFORM_COLOR (0xbf6950ff)

#define GRASS_TOP_COLOR (0x329114ff)

void render_grass_platform(Vector2 position, Vector2 offset) {
    int top_offset = 6;
    int height = 12;
    int top_height = 5;

    Vector2 top_pos = position;
    Vector2 bottom_pos = position;

    Vector2 top_size = (Vector2){.x = TILE_SIZE, .y = top_height};
    Vector2 bottom_size = (Vector2){.x = TILE_SIZE - top_offset, .y = height};

    bottom_pos.x += top_offset / 2; // NOLINT
    DrawRectangleV(Vector2Add(bottom_pos, offset), bottom_size, GetColor(PLATFORM_COLOR));
    DrawRectangleV(Vector2Add(position, offset), top_size, GetColor(GRASS_TOP_COLOR));
}

void render_platform(Vector2 position, Vector2 offset) {
    int top_offset = 6;
    int height = 12;
    int top_height = 5;

    Vector2 top_pos = position;
    Vector2 bottom_pos = position;

    Vector2 top_size = (Vector2){.x = TILE_SIZE, .y = top_height};
    Vector2 bottom_size = (Vector2){.x = TILE_SIZE - top_offset, .y = height};

    
    bottom_pos.x += top_offset / 2; // NOLINT
    DrawRectangleV(Vector2Add(bottom_pos, offset), bottom_size, GetColor(PLATFORM_COLOR));
    DrawRectangleV(Vector2Add(top_pos, offset), top_size, GetColor(PLATFORM_TOP_COLOR));
}

void render_tile(Tile* tile, Vector2 offset) {

    switch (tile->kind) {
        case Platform:
            render_platform(tile->position, offset);
            break;

        case GrassPlatform:
            render_grass_platform(tile->position, offset);
            break;

        case EndTile:
            break;
    }
}

void draw_tile(Tiles kind, Vector2 position, Vector2 offset) {
    switch (kind) {
        case Platform:
            render_platform(position, offset);
            break;

        case GrassPlatform:
            render_grass_platform(position, offset);
            break;

        case EndTile:
            break;
    }
}
