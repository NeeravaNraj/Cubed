#include <unistd.h>
#include <raylib.h>

#include "inc/arena.h"
#include "inc/common.h"
#include "inc/entities/player.h"
#include "inc/hashmap.h"
#include "inc/tilemap.h"
#include "inc/tiles.h"


#define SKY_COLOR ((Color) { .r = 130, .g = 200, .b = 230, .a = 255 })

struct World {
    Player player;
    Tilemap tilemap;
} world;

void init() {
    init_player(&world.player);
    tilemap_init(&world.tilemap);
}

int main() {
    init();
    arena_init();

    /* Tile tile = { */
    /*     .position = { */
    /*         .x = 200, */
    /*         .y = 600, */
    /*     }, */
    /*     .kind = Platform, */
    /* }; */
    /* Tile tile2 = { */
    /*     .position = { */
    /*         .x = 400, */
    /*         .y = 550, */
    /*     }, */
    /*     .kind = GrassPlatform, */
    /* }; */

    world.player.entity.position.x = 100;
    world.player.entity.position.y = 400;
    int x = 0;
    for (int i = 1; i <= 10; ++i) {
        Tile* tile = arena_alloc(sizeof(Tile));
        tile->position.x = x += 60;
        tile->position.y = 600 - i * 25;
        tile->kind = Platform;
        tilemap_add_tile(&world.tilemap, tile);
    }
    /* x = 200; */
    /* for (int i = 1; i <= 10; ++i) { */
    /*     Tile* tile = arena_alloc(sizeof(Tile)); */
    /*     tile->position.x = x += 200; */
    /*     tile->position.y = 600; */
    /*     tile->kind = GrassPlatform; */
    /*     tilemap_add_tile(&world.tilemap, tile); */
    /* } */
    hashmap_print(&world.tilemap.map);

    InitWindow(WIDTH, HEIGHT, "Cubed");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        ClearBackground(SKY_COLOR);
        float dt = GetFrameTime();

        switch (GetKeyPressed()) {
            case KEY_SPACE: {
                float velocity = world.player.entity.velocity.y;
                world.player.entity.velocity.y = minf(velocity - 7, 5);
                break;
            }
        }

        if (IsKeyDown(KEY_A)) {
            world.player.movement[0] = true;
        } else {
            world.player.movement[0] = false;
        }

        if (IsKeyDown(KEY_D)) {
            world.player.movement[1] = true;
        } else {
            world.player.movement[1] = false;
        }


        world.player.entity.update(&world.player.entity, &world.tilemap, dt);
        BeginDrawing();
            world.player.entity.render(&world.player.entity);
            tilemap_render(&world.tilemap);
        EndDrawing();
    }
    
    CloseWindow();

    arena_deinit();
    return 0;
}
