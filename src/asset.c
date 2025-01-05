#include <stdint.h>
#include "inc/asset.h"
#include "inc/arena.h"
#include "inc/common.h"
#include "inc/hashmap.h"
#include "inc/sprites.h"

HashMap* assets = NULL;

SpriteSheet* get_asset(const char* name) {
    const Entry* entry = hashmap_get(assets, name);
    return entry != NULL ? entry->value : NULL;
}

void load_assets() {
    assets = arena_alloc(sizeof(HashMap));
    SpriteSheet* grass_tiles = arena_alloc(sizeof(SpriteSheet));
    SpriteSheet* stone_tiles = arena_alloc(sizeof(SpriteSheet));
    SpriteSheet* small_decor = arena_alloc(sizeof(SpriteSheet));
    hashmap_init(assets);

    *grass_tiles = spritesheet_load("assets/tiles/grass.png", 9, TILE_SIZE, TILE_SIZE);
    *stone_tiles = spritesheet_load("assets/tiles/stone.png", 9, TILE_SIZE, TILE_SIZE);
    *small_decor = spritesheet_load("assets/decor/small_decor.png", 4, TILE_SIZE, TILE_SIZE);

    hashmap_insert(assets, "grass_tiles", grass_tiles);
    hashmap_insert(assets, "stone_tiles", stone_tiles);
    hashmap_insert(assets, "small_decor", small_decor);
}
