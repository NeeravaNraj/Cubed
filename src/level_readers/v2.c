#include <stdio.h>
#include "../inc/level.h"

void header_reader_v2(FILE* file, Headers* headers) {
    size_t read = fread(&headers->spawn, sizeof(Vector2), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.spawn' properly from file.");

    read = fread(&headers->end, sizeof(Vector2), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.end' properly from file.");

    read = fread(&headers->tile_length, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.tile_length' properly from file.");

    read = fread(&headers->offgrid_tile_length, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.offgrid_tile_length' properly from file.");

    read = fread(&headers->moving_platforms_length, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.offgrid_tile_length' properly from file.");
}

void tile_reader_v2(FILE* file, Headers* headers, World* world) {
    for (size_t i = 0; i < headers->tile_length; ++i) {
        Tile* tile = arena_alloc(sizeof(Tile));
        size_t read = fread(tile, sizeof(Tile), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'Tile' properly from file.");
        tilemap_add_tile(&world->tilemap, tile);
    }

    for (size_t i = 0; i < headers->offgrid_tile_length; ++i) {
        Tile* tile = arena_alloc(sizeof(Tile));
        size_t read = fread(tile, sizeof(Tile), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'Offgrid.Tile' properly from file.");
        offgrid_add_tile(&world->offgrid_tiles, tile);
    }

    for (size_t i = 0; i < headers->moving_platforms_length; ++i) {
        size_t platform_id = moving_platforms_add(&world->moving_platforms, vec2(0, 0), vec2(0, 0), 0);
        MovingPlatform* platform = &world->moving_platforms.platforms[platform_id];
        size_t total_tiles = 0;

        size_t read = fread(&platform->start_position, sizeof(platform->start_position), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'MovingPlatform.start_position' properly from file.");
        read = fread(&platform->end_position, sizeof(platform->end_position), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'MovingPlatform.end_position' properly from file.");
        read = fread(&platform->size, sizeof(platform->size), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'MovingPlatform.size' properly from file.");
        read = fread(&platform->velocity, sizeof(platform->velocity), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'MovingPlatform.velocity' properly from file.");
        read = fread(&platform->speed, sizeof(platform->speed), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'MovingPlatform.speed' properly from file.");
        read = fread(&total_tiles, sizeof(size_t), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'MovingPlatform.tiles.length' properly from file.");

        for (size_t j = 0; j < total_tiles; ++j) {
            Tile* tile = arena_alloc(sizeof(Tile));
            size_t read = fread(tile, sizeof(Tile), 1, file);
            handle_err(read != 1, NULL, "ERROR: Could not read 'MovingPlatform.Tile' properly from file.");
            moving_platforms_add_tile(&world->moving_platforms, platform_id, tile);
        }
    }
}

void properties_reader_v2(FILE* file, Properties* properties) {
    size_t read = fread(&properties->level_name_len, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Properties.level_name_len' properly from file.");

    char* name = arena_alloc(properties->level_name_len + 1);
    name[properties->level_name_len] = '\0';

    read = fread(name, sizeof(char), properties->level_name_len, file);
    handle_err(read != properties->level_name_len, NULL, "ERROR: Could not read 'Properties.level_name' properly from file.");
    properties->level_name = name;
}
