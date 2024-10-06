#include <stdio.h>
#include <unistd.h>
#include "inc/arena.h"
#include "inc/level.h"
#include "inc/offgrid.h"
#include "inc/tiles.h"
#include "inc/world.h"
#include "inc/raylib.h"
#include "inc/common.h"
#include "inc/tilemap.h"
#include "inc/entities.h"
#include "inc/ui/editor.h"
#include "inc/entities/player.h"

float FRAME_RATE = 60;

World world;
Assets assets;
EditorState editor_state;

void init(char* level_path) {
    world.edit_mode = !false;
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
    offgrid_init(&world.offgrid_tiles);
    editor_init(&editor_state, &world);
    player_init(&world.player);

    if (level_path != NULL) {
        read_level(level_path, &world);
    }

    world.player.entity.position = world.spawn;
}

void handle_events(float dt) {
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
            player_jump(&world.player);
        }
    }
}

void update(float dt) {
    float t = 0.1;
    if (!world.edit_mode) {
        Entity* player = &world.player.entity;
        player_update(player, &world.tilemap, world.camera.offset, dt);
        world.camera.offset.x = lerpf(t, world.camera.offset.x, -player->position.x + (float)WIDTH / 2 - 250);
        world.camera.offset.y = lerpf(t, world.camera.offset.y, -player->position.y + (float)HEIGHT / 2);

        if (player->position.y >= TILE_SIZE * 30) {
            player->position = world.spawn;
        }
    }
}

void render(float dt) {
    int fps = GetFPS();
    char buffer[20];
    sprintf(buffer, "%d", fps); 

    if (!world.edit_mode) {
        tilemap_render(&world.tilemap, world.camera.offset);
        offgrid_render(&world.offgrid_tiles, world.camera.offset);
        player_render(&world.player.entity, world.camera.offset);
    } else {
        editor_render(&editor_state);
    }
    /* DrawText(buffer, 0, HEIGHT - 20, 16, BLACK); */
}

int main(int argc, char** argv) {
    char* level_path = NULL;
    if (argc > 1) level_path = argv[1];

    InitWindow(WIDTH, HEIGHT, "Cubed");
    arena_init();
    init(level_path);

    SetTargetFPS(FRAME_RATE);
    while (!WindowShouldClose()) {
        ClearBackground(SKY_COLOR);
        float dt = GetFrameTime();
        handle_events(dt);
        update(dt);
        BeginDrawing();
            draw_tile(SpawnPoint, 0, world.spawn, world.camera.offset);
            render(dt);
        EndDrawing();
    }
    
    CloseWindow();

    offgrid_deinit(&world.offgrid_tiles);
    arena_deinit();
    return 0;
}
