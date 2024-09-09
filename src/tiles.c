#include "inc/tiles.h"
#include "inc/common.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define GRASS_TOP_COLOR (0x329114ff)
#define PLATFORM_TOP_COLOR (0x642d0aff)
#define PLATFORM_COLOR (0xbf6950ff)

#define SPAWN_TOP_COLOR (0xafafafff)
#define SPAWN_COLOR (0x4f4f4fff)


void render_grass_platform(Vector2 position, Vector2 offset) {
    int top_offset = 6;
    int height = 16;
    int top_height = 6;

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
    int height = 16;
    int top_height = 6;

    Vector2 top_pos = position;
    Vector2 bottom_pos = position;

    Vector2 top_size = (Vector2){.x = TILE_SIZE, .y = top_height};
    Vector2 bottom_size = (Vector2){.x = TILE_SIZE - top_offset, .y = height};

    
    bottom_pos.x += top_offset / 2; // NOLINT
    DrawRectangleV(Vector2Add(bottom_pos, offset), bottom_size, GetColor(PLATFORM_COLOR));
    DrawRectangleV(Vector2Add(top_pos, offset), top_size, GetColor(PLATFORM_TOP_COLOR));
}

void render_spawn(Vector2 position, Vector2 offset) {
    int flag_pole_height = TILE_SIZE * 3;
    DrawTriangle(
        (Vector2) {
            .x = position.x + offset.x,
            .y = (position.y + offset.y) - (flag_pole_height - (float)TILE_SIZE / 2)
        },
        (Vector2) { 
            .x = position.x + (float)TILE_SIZE / 2 + offset.x,
            .y = (position.y + offset.y) - (flag_pole_height - (float)TILE_SIZE / 4)
        },
        (Vector2) { 
            .x = position.x + offset.x,
            .y = (position.y + offset.y) - flag_pole_height
        },
        RED
    );

    Vector2 top_pos = position;
    top_pos.y -= flag_pole_height;
    DrawLineEx(Vector2Add(position, offset), Vector2Add(top_pos, offset), 2, BLACK);
}

void render_tile(Tile* tile, Vector2 offset) {

    switch (tile->kind) {
        case Platform:
            render_platform(tile->position, offset);
            break;

        case GrassPlatform:
            render_grass_platform(tile->position, offset);
            break;

        case SpawnPoint:
            render_spawn(tile->position, offset);
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

        case SpawnPoint:
            render_spawn(position, offset);
            break;

        case EndTile:
            break;
    }
}
