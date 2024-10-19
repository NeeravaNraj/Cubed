#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/level.h"
#include "inc/hashmap.h"
#include "inc/tilemap.h"
#include "inc/tiles.h"
#include "inc/vector.h"
#include "inc/world.h"


void handle_err(bool cond, char* loc, char* message) {
    if (cond) {
        fprintf(stderr, "%s\n", message); 
        if (loc != NULL) perror(loc);
        exit(EXIT_FAILURE);
    }
}

Headers level_header_reader(FILE* file) {
    Headers headers;

    size_t read = fread(&headers.magic, sizeof(int), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.magic' properly from file.");
    handle_err(headers.magic != MAGIC, NULL, "ERROR: Invalid level file\n");


    read = fread(&headers.version, sizeof(int), 1, file);
    handle_err(read != 1, NULL, "ERROR: Could not read 'Headers.version' properly from file.");
    
    printf("CUBED_INFO: Attempting to read level file with version '%d'\n", headers.version);
    switch (headers.version) {
        case 0:
            header_reader_v0(file, &headers);
            break;

        case 1:
            header_reader_v1(file, &headers);
            break;

        case 2:
            header_reader_v2(file, &headers);
            break;

        default:
            handle_err(true, NULL, "ERROR: Unknown version encountered.");
            break;
    }

    return headers;
}

void level_tile_reader(FILE* file, Headers* headers, World* world) {
    switch (headers->version) {
        case 0:
            tile_reader_v0(file, headers, world);
            break;

        case 1:
            tile_reader_v1(file, headers, world);
            break;

        case 2:
            tile_reader_v2(file, headers, world);
            break;
    }
}

Properties level_properties_reader(FILE* file, Headers* headers) {
    Properties properties;
    switch (headers->version) {
        case 0:
            properties_reader_v0(file, &properties);
            break;

        case 1:
            properties_reader_v1(file, &properties);
            break;

        case 2:
            properties_reader_v2(file, &properties);
            break;
    }

    return properties;
}

void level_header_writer(FILE* file, World* world, size_t tile_count) {
    Headers headers = {
        .magic = MAGIC,
        .version = VERSION,
        .spawn = world->spawn,
        .end = world->end,
        .tile_length = tile_count,
        .offgrid_tile_length = Vec_length(world->offgrid_tiles.tiles),
        .moving_platforms_length = Vec_length(world->moving_platforms.platforms),
    };

    size_t written = fwrite(&headers, sizeof(Headers), 1, file);
    handle_err(written != 1, NULL, "ERROR: Could not write 'Headers' properly to file.");
}

void level_tile_writer(FILE* file, World* world) {
    Entry* tile_entry;
    HashMapIterator it;
    hashmap_init_iterator(&it, &world->tilemap.map);
    while ((tile_entry = hashmap_next_entry(&it))) {
        Tile* tile = tile_entry->value;
        size_t written = fwrite(tile, sizeof(Tile), 1, file);
        handle_err(written != 1, NULL, "ERROR: Could not write 'Tile' properly to file.");
    }

    size_t offgrid_tile_count = Vec_length(world->offgrid_tiles.tiles);
    for (size_t i = 0; i < offgrid_tile_count; ++i) {
        Tile* tile = world->offgrid_tiles.tiles[i];
        size_t written = fwrite(tile, sizeof(Tile), 1, file);
        handle_err(written != 1, NULL, "ERROR: Could not write 'Offgrid-Tile' properly to file.");
    }
    
    size_t moving_platforms_count =  Vec_length(world->moving_platforms.platforms);
    for (size_t i = 0; i < moving_platforms_count; ++i) {
        MovingPlatform* platform = &world->moving_platforms.platforms[i];
        size_t total_tiles = Vec_length(platform->tiles);

        size_t written = fwrite(&platform->start_position, sizeof(platform->start_position), 1, file);
        handle_err(written != 1, NULL, "ERROR: Could not write 'MovingPlatform.start_position' properly to file.");
        written = fwrite(&platform->end_position, sizeof(platform->end_position), 1, file);
        handle_err(written != 1, NULL, "ERROR: Could not write 'MovingPlatform.end_position' properly to file.");
        written = fwrite(&platform->size, sizeof(platform->size), 1, file);
        handle_err(written != 1, NULL, "ERROR: Could not write 'MovingPlatform.size' properly to file.");
        written = fwrite(&platform->velocity, sizeof(platform->velocity), 1, file);
        handle_err(written != 1, NULL, "ERROR: Could not write 'MovingPlatform.velocity' properly to file.");
        written = fwrite(&platform->speed, sizeof(platform->speed), 1, file);
        handle_err(written != 1, NULL, "ERROR: Could not write 'MovingPlatform.speed' properly to file.");
        written = fwrite(&total_tiles, sizeof(size_t), 1, file);
        handle_err(written != 1, NULL, "ERROR: Could not write 'MovingPlatform.tiles.length' properly to file.");

        for (size_t j = 0; j < total_tiles; ++j) {
            Tile* tile = platform->tiles[j];
            written = fwrite(tile, sizeof(Tile), 1, file);
            handle_err(written != 1, NULL, "ERROR: Could not write 'MovingPlatform.Tile' properly to file.");
        }
    }
}

void level_properties_writer(FILE* file, char* name, size_t name_len) {
    Properties properties = {
        .level_name_len = name_len,
        .level_name = name,
    };

    size_t written = fwrite(&properties.level_name_len, sizeof(size_t), 1, file);
    handle_err(written != 1, NULL, "ERROR: Could not write 'Properties.level_name_len' properly to file.");
    written = fwrite(name, sizeof(char), name_len, file);
    handle_err(written != name_len, NULL, "ERROR: Could not write 'Properties.level_name' properly to file.");
}

void write_level(char *name, World* world) {
    size_t name_len = strlen(name);
    size_t tile_count = hashmap_len(&world->tilemap.map);

    char* filename = malloc(name_len + 5);
    sprintf(filename, "%s.cdb", name); 
    FILE* file = fopen(filename, "wb");
    handle_err(file == NULL, "write_level", "ERROR: Failed to write level file.");

    level_header_writer(file, world, tile_count);
    level_tile_writer(file, world);
    level_properties_writer(file, name, name_len);

    fclose(file);
    free(filename);
}


void read_level(char *filename, World* world) {
    FILE* file = fopen(filename, "rb");
    handle_err(file == NULL, "read_level", "ERROR: Failed to read level file.");

    Headers headers = level_header_reader(file);
    world->spawn = headers.spawn;
    world->end = headers.end;
    level_tile_reader(file, &headers, world);
    Properties properties = level_properties_reader(file, &headers);

    printf("Finished reading level: '%s'\n", properties.level_name);

cleanup:
    fclose(file);
}
