#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "inc/level.h"
#include "inc/hashmap.h"
#include "inc/tiles.h"

void handle_err(bool cond, char* loc, char* message) {
    if (!cond) {
        fprintf(stderr, "%s\n", message); 
        if (loc != NULL) perror(loc);
        exit(EXIT_FAILURE);
    }
}

void write_level(char *name, Tilemap *map) {
    size_t name_len = strlen(name);
    size_t tile_count = hashmap_len(&map->map);

    char* filename = malloc(name_len + 5);
    sprintf(filename, "%s.cdb", name); 
    FILE* file = fopen(filename, "wb");
    handle_err(file != NULL, "write_level", "ERROR: Failed to write level file.");

    Headers headers = {
        .magic = MAGIC,
        .version = VERSION,
        .tile_length = tile_count,
    };

    Properties properties = {
        .level_name_len = name_len,
        .level_name = name,
    };

    size_t written = fwrite(&headers, sizeof(Headers), 1, file);
    handle_err(written == 1, NULL, "ERROR: Could not write 'Headers' properly to file.");

    Entry* tile_entry;
    HashMapIterator it;
    hashmap_init_iterator(&it, &map->map);
    while ((tile_entry = hashmap_next_entry(&it))) {
        Tile* tile = tile_entry->value;
        written = fwrite(tile, sizeof(Tile), 1, file);
        handle_err(written == 1, NULL, "ERROR: Could not write 'Tile' properly to file.");
    }

    written = fwrite(&properties.level_name_len, sizeof(size_t), 1, file);
    handle_err(written == 1, NULL, "ERROR: Could not write 'Properties.level_name_len' properly to file.");
    written = fwrite(name, sizeof(char), name_len, file);
    handle_err(written == name_len, NULL, "ERROR: Could not write 'Properties.level_name' properly to file.");

    fclose(file);
    free(filename);
}
