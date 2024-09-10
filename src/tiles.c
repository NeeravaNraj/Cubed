#include "inc/tiles.h"
#include "inc/common.h"
#include "inc/raylib.h"
#include "inc/raymath.h"

#define GRASS_TOP_COLOR (0x329114ff)
#define PLATFORM_TOP_COLOR (0x642d0aff)
#define PLATFORM_COLOR (0xbf6950ff)

#define SPAWN_TOP_COLOR (0xafafafff)
#define SPAWN_COLOR (0x4f4f4fff)


void render_grass_platform(Vector2 position, Vector2 offset) {
    Texture2D asset = assets.grass.textures[0];
    Rectangle src = {
        .x = 0,
        .y = 0,
        .width = asset.width,
        .height = asset.height,
    };
    Rectangle dest = {
        .x = position.x + offset.x,
        .y = position.y + offset.y,
        .height = TILE_SIZE,
        .width = TILE_SIZE,
    };
    DrawTexturePro(asset, src, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE);
}

void render_platform(Vector2 position, Vector2 offset) {
    Texture2D asset = assets.stone.textures[1];
    Rectangle src = {
        .x = 0,
        .y = 0,
        .width = asset.width,
        .height = asset.height,
    };
    Rectangle dest = {
        .x = position.x + offset.x,
        .y = position.y + offset.y,
        .height = TILE_SIZE,
        .width = TILE_SIZE,
    };
    DrawTexturePro(asset, src, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE);
}

void render_end(Vector2 position, Vector2 offset) {
    Texture2D asset = assets.end.textures[0];
    Rectangle src = {
        .x = 16,
        .y = 0,
        .width = 8,
        .height = asset.height,
    };
    Rectangle dest = {
        .x = position.x + offset.x,
        .y = position.y + offset.y,
        .height = TILE_SIZE,
        .width = TILE_SIZE,
    };
    DrawTexturePro(asset, src, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE);
}

void render_spawn(Vector2 position, Vector2 offset) {
    int flag_pole_height = TILE_SIZE * 3;
    int flag_offset = 10;
    DrawTriangle(
        (Vector2) {
            .x = position.x + offset.x + flag_offset,
            .y = (position.y + offset.y) - (flag_pole_height - (float)TILE_SIZE / 2)
        },
        (Vector2) { 
            .x = position.x + (float)TILE_SIZE / 2 + offset.x + flag_offset,
            .y = (position.y + offset.y) - (flag_pole_height - (float)TILE_SIZE / 4)
        },
        (Vector2) { 
            .x = position.x + offset.x + flag_offset,
            .y = (position.y + offset.y) - flag_pole_height
        },
        RED
    );

    Vector2 flag_pos = position;
    flag_pos.x  += flag_offset;
    Vector2 top_pos = position;
    top_pos.x += flag_offset;
    top_pos.y -= flag_pole_height;
    DrawLineEx(Vector2Add(flag_pos, offset), Vector2Add(top_pos, offset), 2, BLACK);
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

        case EndPoint:
            render_end(tile->position, offset);

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

        case EndPoint:
            render_end(position, offset);

        case EndTile:
            break;
    }
}
