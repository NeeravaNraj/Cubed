#include <stdio.h>
#include <assert.h>
#include "inc/tiles.h"
#include "inc/arena.h"
#include "inc/common.h"
#include "inc/asset.h"
#include "inc/components/box_collider.h"
#include "inc/hashmap.h"
#include "inc/physics.h"
#include "inc/scene.h"
#include "inc/sprites.h"
#include "inc/game_object.h"
#include "inc/raylib/raylib.h"
#include "inc/components/rigid_body.h"
#include "inc/components/sprite_renderer.h"

#define COORD_DELIM (',')

HashMap* tilemap = NULL;

Sprite tile_get_sprite(Tiles kind, char variant) {
    switch (kind) {
        case GrassPlatform: {
            SpriteSheet* sheet = get_asset("grass_tiles");
            return spritesheet_get_sprite(sheet, (int)variant);
        }
    
        case StonePlatform: {
            SpriteSheet* sheet = get_asset("stone_tiles");
            return spritesheet_get_sprite(sheet, (int)variant);
        }

        case SmallDecor: {
            SpriteSheet* sheet = get_asset("small_decor");
            return spritesheet_get_sprite(sheet, (int)variant);
        }

        default:
            printf("ERROR: Unknown tile kind %d\n", kind);
            assert(0);
            break;
    }
}

const char* tile_get_key(Vector2 position) {
    int tile_x = position.x / TILE_SIZE;
    int tile_y = position.y / TILE_SIZE;
    size_t x_len = snprintf(NULL, 0, "%d", tile_x);
    size_t y_len = snprintf(NULL, 0, "%d", tile_y);
    int total = x_len + y_len + 2;
    char* key = arena_alloc(total);
    int printed = snprintf(
        key, total,
        "%d%c%d",
        tile_x, COORD_DELIM, tile_y
    );
    assert(printed == total- 1);

    return key;
}

bool tile_exists(Vector2 position) {
    int tile_x = position.x / TILE_SIZE;
    int tile_y = position.y / TILE_SIZE;
    char buffer[24] = {0};
    int to_print = snprintf(
        buffer, sizeof(buffer), "%d%c%d",
        tile_x, COORD_DELIM, tile_y 
    );
    assert(to_print < 24 || to_print == 0);

    return hashmap_get(tilemap, buffer) != NULL;
}

void tile_create(Vector2 position, Tiles kind, char variant) {
    if (tilemap == NULL) {
        tilemap = arena_alloc(sizeof(HashMap));
        hashmap_init(tilemap);
    }

    Vector2 tile_coords = to_tile_space(position);
    if (tile_exists(tile_coords)) return;

    Sprite sprite = tile_get_sprite(kind, variant);
    const char* key = tile_get_key(tile_coords);

    GameObject* go = go_create();

    go->transform.position = tile_coords;
    go->transform.scale = vec2(TILE_SIZE, TILE_SIZE);
    go->transform.rotation = 0;

    SpriteRenderer* renderer = spriterenderer_create(go, sprite);
    go_add_component(go, &renderer->component);

    RigidBody* rigid_body = rigidbody_create(go);
    rigid_body->friction = 0.1;
    rigid_body->fixed_rotation = true;
    go_add_component(go, &rigid_body->component);

    BoxCollider* box_collider = boxcollider_create(go);
    box_collider->half_size.x = (float)TILE_SIZE / 2;
    box_collider->half_size.y = (float)TILE_SIZE / 2;
    go_add_component(go, &box_collider->component);

    physics_add(go);

    hashmap_insert(tilemap, key, go);
    scene_add_game_object(*go);
}

Vector2 resolve_tile_position(Vector2 pos) {
     if (pos.x < 0 && (int)pos.x % TILE_SIZE != 0) {
        int rem = (int)(pos.x) % TILE_SIZE;
        pos.x = pos.x - rem - TILE_SIZE;
    }

    if (pos.y < 0 && (int)pos.y % TILE_SIZE != 0) {
        int rem = (int)(pos.y) % TILE_SIZE;
        pos.y = pos.y - rem - TILE_SIZE;
    }

    return pos;
}
