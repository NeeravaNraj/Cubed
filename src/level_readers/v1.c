#include <stdio.h>
#include "../inc/level.h"

void header_reader_v1(FILE* file, Headers* headers) {
    size_t read = fread(&headers->spawn, sizeof(Vector2), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.spawn' properly from file.");

    read = fread(&headers->end, sizeof(Vector2), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.end' properly from file.");

    read = fread(&headers->tile_length, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.tile_length' properly from file.");

    read = fread(&headers->offgrid_tile_length, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.offgrid_tile_length' properly from file.");

    headers->moving_platforms_length = 0;
}

void tile_reader_v1(FILE* file, Headers* headers, World* world) {
    for (size_t i = 0; i < headers->tile_length; ++i) {
        Tile* tile = arena_alloc(sizeof(Tile));
        size_t read = fread(tile, sizeof(Tile), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'Tile' properly from file.");
        tilemap_add_tile(&world->tilemap, tile);
    }

    for (size_t i = 0; i < headers->offgrid_tile_length; ++i) {
        Tile* tile = arena_alloc(sizeof(Tile));
        size_t read = fread(tile, sizeof(Tile), 1, file);
        handle_err(read != 1, NULL, "ERROR: Could not read 'Tile' properly from file.");
        offgrid_add_tile(&world->offgrid_tiles, tile);
    }
}

void properties_reader_v1(FILE* file, Properties* properties) {
    size_t read = fread(&properties->level_name_len, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Properties.level_name_len' properly from file.");

    char* name = arena_alloc(properties->level_name_len + 1);
    name[properties->level_name_len] = '\0';

    read = fread(name, sizeof(char), properties->level_name_len, file);
    handle_err(read != properties->level_name_len, NULL, "ERROR: Could not read 'Properties.level_name' properly from file.");
    properties->level_name = name;
}
