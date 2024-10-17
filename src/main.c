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
#include "inc/moving_platforms.h"

float FRAME_RATE = 120;
float ticks = 0;

Font font;
Assets assets;

World world;
Player player;
EditorState editor_state;

void init(char* level_path) {
    world.edit_mode = !false;
    world.camera = (Camera2D){
        .zoom = 1,
        .offset = { .x = 0, .y = 0 },
        .target = {
            .x =  -player.entity.position.x + (float)WIDTH / 2 - 250,
            .y =  -player.entity.position.y + (float)HEIGHT / 2,
        },
    };
    load_assets(&assets);
    font = LoadFontEx("assets/font/main.ttf", 128, 0, 256);
    tilemap_init(&world.tilemap);
    offgrid_init(&world.offgrid_tiles);
    moving_platforms_init(&world.moving_platforms);
    player_init(&player);
    editor_init(&editor_state, &world);

    if (level_path != NULL) {
        read_level(level_path, &world);
    }

    reset();
}

void reset() {
    player.entity.position = world.spawn;
    player.entity.position.y -= TILE_SIZE;
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
            player.movement[0] = true;
        } else {
            player.movement[0] = false;
        }

        if (IsKeyDown(KEY_D)) {
            player.movement[1] = true;
        } else {
            player.movement[1] = false;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            player_jump(&player);
        }
    }
}

void update(float dt) {
    float t = 0.1;
    if (!world.edit_mode) {
        Entity* player_entity = &player.entity;
        moving_platforms_update(&world.moving_platforms, dt);
        player_update(&player, &world, world.camera.offset, dt);
        world.camera.offset.x = lerpf(t, world.camera.offset.x, -player_entity->position.x + (float)WIDTH / 2 - 250);
        world.camera.offset.y = lerpf(t, world.camera.offset.y, -player_entity->position.y + (float)HEIGHT / 2);

        if (player_entity->position.y >= TILE_SIZE * 30) {
            reset();
        }
    }
}

void render(float dt) {
    /* int fps = GetFPS(); */
    /* char buffer[20]; */
    /* sprintf(buffer, "%d", fps);  */

    if (!world.edit_mode) {
        tilemap_render(&world.tilemap, world.camera.offset);
        moving_platforms_render(&world.moving_platforms, world.camera.offset);
        offgrid_render(&world.offgrid_tiles, world.camera.offset);
        player_render(&player, world.camera.offset);
    } else {
        editor_render(&editor_state);
    }
    /* DrawText(buffer, 0, HEIGHT - 20, 16, BLACK); */
}

int main(int argc, char** argv) {
    char* level_path = NULL;
    if (argc > 1) level_path = argv[1];

    float accumulator = 0;
    InitWindow(WIDTH, HEIGHT, "Cubed");
    arena_init();
    init(level_path);
 
    SetTargetFPS(FRAME_RATE);
    while (!WindowShouldClose()) {
        ClearBackground(SKY_COLOR);
        float dt = minf(GetFrameTime(), 0.25);
        handle_events(dt);

        accumulator += dt;
        while (accumulator >= FIXED_UPDATE_MS) {
            update(FIXED_UPDATE_MS);
            accumulator -= FIXED_UPDATE_MS;
        }

        BeginDrawing();
            draw_tile(SpawnPoint, 0, world.spawn, world.camera.offset);
            render(dt);
        EndDrawing();
    }
    
    CloseWindow();

    /* moving_platforms_deinit(&plt); */
    offgrid_deinit(&world.offgrid_tiles);
    arena_deinit();
    return 0;
}
