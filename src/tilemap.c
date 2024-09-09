#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <raylib.h>
#include <unistd.h>
#include "inc/tilemap.h"
#include "inc/arena.h"
#include "inc/common.h"
#include "inc/hashmap.h"
#include "inc/tiles.h"

#define COORD_DELIM (',')

Vector2 NEIGHBOUR_OFFSETS[9] = {
    {.x = -1, .y = -1}, {.x = 0, .y = -1}, {.x = 1, .y = -1},
    {.x = -1, .y = 0}, {.x = 0, .y = 0}, {.x = 1, .y = 0},
    {.x = -1, .y = 1}, {.x = 0, .y = 1}, {.x = 1, .y = 1},
};

Vector2 TILE_SIZEV = { .x = TILE_SIZE, .y = TILE_SIZE };

void tilemap_init(Tilemap* tm) {
    hashmap_init(&tm->map);
    tm->around = arena_alloc(sizeof(Tiles) * 9);
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

    tile->position.x = tile_x * TILE_SIZE;
    tile->position.y = tile_y * TILE_SIZE;
    printf("tile: %f, %f\n", tile->position.x, tile->position.y);
    hashmap_insert(&tm->map, key, tile);
#undef TOTAL
}

Tile** tilemap_tiles_around(Tilemap* tm, Vector2 position) {
    int tile_x = position.x / TILE_SIZE;
    int tile_y = position.y / TILE_SIZE;

    assert(tile_x < INT_MAX && tile_x > INT_MIN);
    assert(tile_y < INT_MAX && tile_y > INT_MIN);
    int counter = 0;
    char buffer[24] = {0};
    /* printf("["); */
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

    tm->around[counter] = NULL;

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
