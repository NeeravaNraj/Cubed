#include <stdio.h>
#include <stdlib.h>
#include "../inc/level.h"

void header_reader_v0(FILE* file, Headers* headers, LevelData* level) {
    size_t read = fread(&headers->tile_length, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.tile_length' properly from file.");
}

void tile_reader_v0(FILE* file, Headers* headers, LevelData* level) {
    /* for (size_t i = 0; i < headers->tile_length; ++i) { */
    /*     Tile* tile = arena_alloc(sizeof(Tile)); */
    /*  */
    /*     size_t read = fread(&tile->kind, sizeof(Tiles), 1, file); */
    /*     handle_err(read != 1, NULL, "ERROR: Could not read 'Tile.kind' properly from file."); */
    /*     read = fread(&tile->variant, sizeof(char), 1, file); */
    /*     handle_err(read != 1, NULL, "ERROR: Could not read 'Tile.variant' properly from file."); */
    /*     read = fread(&tile->position, sizeof(Vector2), 1, file); */
    /*     handle_err(read != 1, NULL, "ERROR: Could not read 'Tile.position' properly from file."); */
    /*     tile->body_id = b2_nullBodyId; */
    /*  */
    /*     tilemap_add_tile(&level->tilemap, tile); */
    /* } */
}

void properties_reader_v0(FILE* file, Properties* properties, LevelData* level) {
    size_t read = fread(&properties->level_name_len, sizeof(size_t), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Properties.level_name_len' properly from file.");

    char* name = calloc(0, properties->level_name_len + 1);
    name[properties->level_name_len] = '\0';

    read = fread(name, sizeof(char), properties->level_name_len, file);
    handle_err(read != properties->level_name_len, NULL, "ERROR: Could not read 'Properties.level_name' properly from file.");
    properties->level_name = name;
}
