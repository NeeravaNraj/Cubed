#include "inc/tiles.h"
#include "inc/common.h"
#include "inc/raylib/raylib.h"
#include "inc/raylib/raymath.h"

#define GRASS_TOP_COLOR (0x329114ff)
#define PLATFORM_TOP_COLOR (0x642d0aff)
#define PLATFORM_COLOR (0xbf6950ff)

#define SPAWN_TOP_COLOR (0xafafafff)
#define SPAWN_COLOR (0x4f4f4fff)


void _render_tile(Texture2D asset, Vector2 position, char variant, Vector2 offset) {
    Rectangle src = {
        .x = variant * TEXTURE_SIZE,
        .y = 0,
        .width = TEXTURE_SIZE,
        .height = TEXTURE_SIZE,
    };
    Rectangle dest = {
        .x = position.x + offset.x,
        .y = position.y + offset.y,
        .height = TILE_SIZE,
        .width = TILE_SIZE,
    };
    DrawTexturePro(asset, src, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE);
}

void render_grass_platform(Vector2 position, char variant, Vector2 offset) {
    Texture2D asset = assets.tile_assets.grass.textures[0];
    _render_tile(asset, position, variant, offset);
}

void render_stone_platform(Vector2 position, char variant, Vector2 offset) {
    Texture2D asset = assets.tile_assets.stone.textures[0];
    _render_tile(asset, position, variant, offset);
}

void render_small_decor(Vector2 position, char variant, Vector2 offset) {
    Texture2D asset = assets.decor_assets.small_decor.textures[0];
    _render_tile(asset, position, variant, offset);
}

void render_end(Vector2 position, char variant, Vector2 offset) {
    /* Texture2D asset = assets.end.textures[0]; */
    /* Rectangle src = { */
    /*     .x = 16, */
    /*     .y = 0, */
    /*     .width = 8, */
    /*     .height = asset.height, */
    /* }; */
    /* Rectangle dest = { */
    /*     .x = position.x + offset.x, */
    /*     .y = position.y + offset.y, */
    /*     .height = TILE_SIZE, */
    /*     .width = TILE_SIZE, */
    /* }; */
    /* DrawTexturePro(asset, src, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE); */
}

void render_spawn(Vector2 position, char variant, Vector2 offset) {
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
        case StonePlatform:
            render_stone_platform(tile->position, tile->variant, offset);
            break;

        case GrassPlatform:
            render_grass_platform(tile->position, tile->variant, offset);
            break;

        case SpawnPoint:
            render_spawn(tile->position, tile->variant, offset);
            break;

        case SmallDecor:
            render_small_decor(tile->position, tile->variant, offset);
            break;

        case EndPoint:
            render_end(tile->position, tile->variant, offset);
            break;

        case EndTile:
            break;
    }
}

void draw_tile(Tiles kind, char variant, Vector2 position, Vector2 offset) {
    switch (kind) {
        case StonePlatform:
            render_stone_platform(position, variant, offset);
            break;

        case GrassPlatform:
            render_grass_platform(position, variant, offset);
            break;

        case SpawnPoint:
            render_spawn(position, variant, offset);
            break;

        case SmallDecor:
            render_small_decor(position, variant, offset);
            break;

        case EndPoint:
            render_end(position, variant, offset);
            break;

        case EndTile:
            break;
    }
}
