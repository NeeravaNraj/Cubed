#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/raylib.h"
#include "inc/tilemap.h"
#include "inc/arena.h"
#include "inc/common.h"
#include "inc/hashmap.h"
#include "inc/tiles.h"

#define COORD_DELIM (',')

enum Sides {
    Top = 1,
    Left = 2,
    Right = 4,
    Down = 8
};

Vector2 NEIGHBOUR_OFFSETS[9] = {
    {.x = -1, .y = -1}, {.x = 0, .y = -1}, {.x = 1, .y = -1},
    {.x = -1, .y = 0}, {.x = 0, .y = 0}, {.x = 1, .y = 0},
    {.x = -1, .y = 1}, {.x = 0, .y = 1}, {.x = 1, .y = 1},
};

Vector2 AUTO_TILE_NEIGHBOURS[4] = {
    {.x = 0, .y = -1},
    {.x = 1, .y = 0},
    {.x = 0, .y = 1},
    {.x = -1, .y = 0}
};

Vector2 TILE_SIZEV = { .x = TILE_SIZE, .y = TILE_SIZE };

void tilemap_init(Tilemap* tm) {
    hashmap_init(&tm->map);
    tm->around = arena_alloc(sizeof(Tile) * 9);
}

void tilemap_add_tile(Tilemap* tm, Tile* tile) {
    int tile_x = tile->position.x / TILE_SIZE;
    int tile_y = tile->position.y / TILE_SIZE;

    size_t x_len = snprintf(NULL, 0, "%d", tile_x);
    size_t y_len = snprintf(NULL, 0, "%d", tile_y);
#define TOTAL (x_len + y_len + 2)
    char* key = arena_alloc(TOTAL);
    int printed = snprintf(
        key, TOTAL,
        "%d%c%d",
        tile_x, COORD_DELIM, tile_y
    );
    assert(printed == TOTAL - 1);
#undef TOTAL

    tile->position.x = tile_x * TILE_SIZE;
    tile->position.y = tile_y * TILE_SIZE;
    hashmap_insert(&tm->map, key, tile);
}

Tile* tilemap_get_tile(Tilemap* tm, Vector2 position) {
    int tile_x = position.x / TILE_SIZE;
    int tile_y = position.y / TILE_SIZE;

    size_t x_len = snprintf(NULL, 0, "%d", tile_x);
    size_t y_len = snprintf(NULL, 0, "%d", tile_y);

#define TOTAL (x_len + y_len + 2)
    char* key = arena_alloc(TOTAL);
    int printed = snprintf(
        key, TOTAL,
        "%d%c%d",
        tile_x, COORD_DELIM, tile_y
    );
    assert(printed == TOTAL - 1);
#undef TOTAL

    const Entry* entry = hashmap_get(&tm->map, key);
    return entry ? entry->value : NULL;
}

bool tilemap_remove_tile(Tilemap* tm, Vector2 position) {
    int tile_x = position.x / TILE_SIZE;
    int tile_y = position.y / TILE_SIZE;

    size_t x_len = snprintf(NULL, 0, "%d", tile_x);
    size_t y_len = snprintf(NULL, 0, "%d", tile_y);
#define TOTAL (x_len + y_len + 2)
    char* key = arena_alloc(TOTAL);
    int printed = snprintf(
        key, TOTAL,
        "%d%c%d",
        tile_x, COORD_DELIM, tile_y
    );
    assert(printed == TOTAL - 1);
#undef TOTAL

    return hashmap_delete(&tm->map, key);
}

Tile** tilemap_tiles_around(Tilemap* tm, Vector2 position) {
    int tile_x = position.x / TILE_SIZE;
    int tile_y = position.y / TILE_SIZE;

    assert(tile_x < INT_MAX && tile_x > INT_MIN);
    assert(tile_y < INT_MAX && tile_y > INT_MIN);
    int counter = 0;
    char buffer[24] = {0};

    for (int i = 0; i < 9; ++i) {
        Vector2 offset = NEIGHBOUR_OFFSETS[i];
        int offset_tile_x = tile_x + offset.x;
        int offset_tile_y = tile_y + offset.y;
        int to_print = snprintf(
            buffer, 24, "%d%c%d",
            offset_tile_x, COORD_DELIM, offset_tile_y
        );
        assert(to_print < 24 || to_print == 0);

        const Entry* entry = hashmap_get(&tm->map, buffer);
        if (entry) {
            tm->around[counter++] = entry->value;
        } 
    }

    if (counter < 9) {
        tm->around[counter] = NULL;
    }

    /* printf("[ "); */
    /* for (int i = 0; i < counter; ++i) { */
    /*     Tile* tile = tm->around[i]; */
    /*     printf("(%f, %f) ", tile->position.x, tile->position.y); */
    /* } */
    /* printf("]\n"); */

    return tm->around;
}

void tilemap_render(Tilemap* tm, Vector2 offset) {
    Entry* tile_entry;
    HashMapIterator it;
    hashmap_init_iterator(&it, &tm->map);
    while ((tile_entry = hashmap_next_entry(&it))) {
        Tile* tile = tile_entry->value;
        render_tile(tile, offset);
    }
}


void tilemap_auto_tile(Tilemap* tm) {
    int variants[9] = {
        1, 0, 2,
        3, 4, 5,
        6, 7, 8
    };
    HashMap auto_tile_map;
    hashmap_init(&auto_tile_map);
    hashmap_insert(&auto_tile_map, "0000", &variants[4]);
    hashmap_insert(&auto_tile_map, "0001", &variants[1]);
    hashmap_insert(&auto_tile_map, "0010", &variants[5]);
    hashmap_insert(&auto_tile_map, "0011", &variants[2]);
    hashmap_insert(&auto_tile_map, "0100", &variants[7]);
    hashmap_insert(&auto_tile_map, "0101", &variants[1]);
    hashmap_insert(&auto_tile_map, "0110", &variants[8]);
    hashmap_insert(&auto_tile_map, "0111", &variants[2]);
    hashmap_insert(&auto_tile_map, "1000", &variants[3]);
    hashmap_insert(&auto_tile_map, "1001", &variants[0]);
    hashmap_insert(&auto_tile_map, "1100", &variants[6]);
    hashmap_insert(&auto_tile_map, "1101", &variants[0]);
    hashmap_insert(&auto_tile_map, "1111", &variants[1]);

    Entry* tile_entry;
    HashMapIterator it;
    hashmap_init_iterator(&it, &tm->map);
    while ((tile_entry = hashmap_next_entry(&it))) {
        Tile* tile = tile_entry->value;

        char sides[5] = "0000\0";
        char key[24] = {0};
        for (int i = 0; i < 4; ++i) {
            Vector2 offset = AUTO_TILE_NEIGHBOURS[i];
            int tile_x = tile->position.x / TILE_SIZE;
            int tile_y = tile->position.y / TILE_SIZE;
            int offset_tile_x = tile_x + offset.x;
            int offset_tile_y = tile_y + offset.y;
            int to_print = snprintf(
                key, 24, "%d%c%d",
                offset_tile_x, COORD_DELIM, offset_tile_y
            );
            assert(to_print < 24 || to_print == 0);
            const Entry* entry = hashmap_get(&tm->map, key);
            if (!entry) { sides[3 - i] = '1'; } 
        }

        const Entry* variant_entry = hashmap_get(&auto_tile_map, sides);
        assert(variant_entry);
        tile->variant = *(char*)variant_entry->value;
    }

    hashmap_deinit(&auto_tile_map);
}
