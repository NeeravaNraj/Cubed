#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/level.h"
#include "inc/box2d/id.h"
#include "inc/common.h"
#include "inc/tiles.h"
#include "inc/hashmap.h"


void handle_err(bool cond, char* loc, char* message) {
    if (cond) {
        fprintf(stderr, "%s\n", message); 
        if (loc != NULL) perror(loc);
        exit(EXIT_FAILURE);
    }
}

Headers level_header_reader(FILE* file, LevelData* level) {
    Headers headers;

    size_t read = fread(&headers.magic, sizeof(int), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.magic' properly from file.");
    handle_err(headers.magic != MAGIC, NULL, "ERROR: Invalid level file\n");


    read = fread(&headers.version, sizeof(int), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.version' properly from file.");
    
    printf("CUBED_INFO: Attempting to read level file with version '%d'\n", headers.version);
    switch (headers.version) {
        case 0:
            header_reader_v0(file, &headers, level);
            break;

        default:
            handle_err(true, NULL, "ERROR: Unknown version encountered.");
            break;
    }

    return headers;
}

void level_tile_reader(FILE* file, Headers* headers, LevelData* level) {
    switch (headers->version) {
        case 0:
            tile_reader_v0(file, headers, level);
            break;

        default:
            handle_err(true, NULL, "ERROR: Unknown version encountered.");
            break;
    }
}

Properties level_properties_reader(FILE* file, Headers* headers, LevelData* level) {
    Properties properties;
    switch (headers->version) {
        case 0:
            properties_reader_v0(file, &properties, level);
            break;

        default:
            handle_err(true, NULL, "ERROR: Unknown version encountered.");
            break;
    }

    return properties;
}

void level_header_writer(FILE* file, size_t tile_count) {
    Headers headers = {
        .magic = MAGIC,
        .version = VERSION,
        .tile_length = tile_count,
    };

    size_t written = fwrite(&headers.magic, sizeof(int), 1, file);
    handle_err(written != 1, NULL, "ERROR: Could not write 'Headers.magic' properly to file.");
    written = fwrite(&headers.version, sizeof(int), 1, file);
    handle_err(written != 1, NULL, "ERROR: Could not write 'Headers.version' properly to file.");
    written = fwrite(&headers.tile_length, sizeof(size_t), 1, file);
    handle_err(written != 1, NULL, "ERROR: Could not write 'Headers.tile_length' properly to file.");
}

/* void level_tile_writer(FILE* file, LevelData* level) { */
/*     Entry* tile_entry; */
/*     HashMapIterator it; */
/*     hashmap_init_iterator(&it, &level->tilemap.map); */
/*     while ((tile_entry = hashmap_next_entry(&it))) { */
/*         Tile* tile = tile_entry->value; */
/*         size_t written = fwrite(&tile->kind, sizeof(Tiles), 1, file); */
/*         handle_err(written != 1, NULL, "ERROR: Could not write 'Tile.kind' properly to file."); */
/*         written = fwrite(&tile->variant, sizeof(char), 1, file); */
/*         handle_err(written != 1, NULL, "ERROR: Could not write 'Tile.variant' properly to file."); */
/*         written = fwrite(&tile->position, sizeof(Vector2), 1, file); */
/*         handle_err(written != 1, NULL, "ERROR: Could not write 'Tile.position' properly to file."); */
/*     } */
/* } */

void level_properties_writer(FILE* file, const char* name, size_t name_len) {
    Properties properties = {
        .level_name_len = name_len,
        .level_name = name,
    };

    size_t written = fwrite(&properties.level_name_len, sizeof(size_t), 1, file);
    handle_err(written != 1, NULL, "ERROR: Could not write 'Properties.level_name_len' properly to file.");
    written = fwrite(name, sizeof(char), name_len, file);
    handle_err(written != name_len, NULL, "ERROR: Could not write 'Properties.level_name' properly to file.");
}

void write_level(const char *name, LevelData* level) {
    size_t name_len = strlen(name);
    size_t tile_count = 0;

    char* filename = malloc(name_len + 5);
    sprintf(filename, "%s%s", name, FILE_EXT); 

    int path_len = sizeof(LEVELS_DIR"/") + name_len + 5;
    char* path = malloc(path_len);
    sprintf(path, "%s/%s", LEVELS_DIR, filename); 

    FILE* file = fopen(path, "wb");
    handle_err(file == NULL, "write_level", "ERROR: Failed to write level file.");

    level_header_writer(file, tile_count);
    /* level_tile_writer(file, level); */
    level_properties_writer(file, name, name_len);

    fclose(file);
    free(path);
    free(filename);
    printf("CUBED_INFO: Level saved.\n");
}


LevelData read_level(const char* path, b2WorldId world_id) {
    FILE* file = fopen(path, "rb");
    LevelData level = {0};

    handle_err(file == NULL, "read_level", "ERROR: Failed to read level file.");

    Headers headers = level_header_reader(file, &level);
    /* level_tile_reader(file, &headers, &level); */
    Properties properties = level_properties_reader(file, &headers, &level);
    level.level_name = properties.level_name;

    printf("Finished reading level: '%s'\n", properties.level_name);

    fclose(file);
    return level;
}
