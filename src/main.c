#include <stdio.h>
#include <unistd.h>
#include "inc/hashmap.h"
#include "inc/raylib.h"
#include "inc/arena.h"
#include "inc/common.h"
#include "inc/entities.h"
#include "inc/level.h"
#include "inc/tilemap.h"
#include "inc/tiles.h"
#include "inc/ui/editor.h"
#include "inc/entities/player.h"
#include "inc/world.h"


World world;
Assets assets;
EditorState editor_state;

void init() {
    world.edit_mode = false;
    world.camera = (Camera2D){
        .zoom = 1,
        .offset = { .x = 0, .y = 0 },
        .target = {
            .x =  -world.player.entity.position.x + (float)WIDTH / 2 - 250,
            .y =  -world.player.entity.position.y + (float)HEIGHT / 2,
        },
    };
    load_assets(&assets);
    tilemap_init(&world.tilemap);
    read_level("test_level.cdb", &world, 0);
    editor_init(&editor_state, &world);
    init_player(&world.player);

    world.player.entity.position = world.spawn;
}

void handle_events() {
    if (IsKeyPressed(KEY_P)) {
        world.edit_mode = !world.edit_mode;
        world.camera.target.x = (float)WIDTH / 2;
        world.camera.target.y = (float)HEIGHT / 2;
        if (world.edit_mode) {
            int tile_x = world.camera.offset.x / TILE_SIZE;
            int tile_y = world.camera.offset.y / TILE_SIZE;
            world.camera.offset.x = tile_x * TILE_SIZE;
            world.camera.offset.y = tile_y * TILE_SIZE;
        }
    }

    if (world.edit_mode) {
        editor_handle_events(&editor_state);
    } else {
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

        if (IsKeyPressed(KEY_SPACE)) {
            float velocity = world.player.entity.velocity.y;
            world.player.entity.velocity.y = minf(velocity - 7, 5);
        }
    }
}

void update(float dt) {
    if (!world.edit_mode) {
        Entity player = world.player.entity;
        world.player.entity.update(&world.player.entity, &world.tilemap, dt);
        float t = 0.1;
        world.camera.offset.x = lerpf(t, world.camera.offset.x, -player.position.x + (float)WIDTH / 2 - 250);
        world.camera.offset.y = lerpf(t, world.camera.offset.y, -player.position.y + (float)HEIGHT / 2);
    }
}

void render() {
    world.player.entity.render(&world.player.entity, world.camera.offset);
    if (!world.edit_mode) {
        tilemap_render(&world.tilemap, world.camera.offset);
    } else {
        editor_render(&editor_state);
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Cubed");
    printf("initted window\n");
    arena_init();
    init();

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        ClearBackground(SKY_COLOR);
        float dt = GetFrameTime();
        handle_events();
        update(dt);
        BeginDrawing();
            draw_tile(SpawnPoint, world.spawn, world.camera.offset);
            render();
        EndDrawing();
    }
    
    CloseWindow();

    arena_deinit();
    return 0;
}
