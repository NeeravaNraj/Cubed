#include <unistd.h>
#include <raylib.h>
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
EditorState editor_state;

void init() {
    world.edit_mode = !false;
    world.screen_offset = (Vector2){ .x = 0, .y = 0 };
    init_player(&world.player);
    tilemap_init(&world.tilemap);
    /* world.spawn = (Vector2){ */
    /*     .x = 100, */
    /*     .y = 500 */
    /* }; */

    if (world.edit_mode) {
        editor_init(&editor_state, &world);
    }
}

void handle_events() {

    if (IsKeyPressed(KEY_P)) {
        world.edit_mode = !world.edit_mode;
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
        world.screen_offset.x = lerpf(t, world.screen_offset.x, -player.position.x + (float)WIDTH / 2 - 250);
        world.screen_offset.y = lerpf(t, world.screen_offset.y, -player.position.y + (float)HEIGHT / 2);
    }
}

void render() {
    world.player.entity.render(&world.player.entity, world.screen_offset);
    if (!world.edit_mode) {
        tilemap_render(&world.tilemap, world.screen_offset);
    } else {
        editor_render(&editor_state);
    }
}

int main() {
    init();
    arena_init();

    /* read_level("test_level.cdb", &world, 0); */

    InitWindow(WIDTH, HEIGHT, "Cubed");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        ClearBackground(SKY_COLOR);
        float dt = GetFrameTime();
        handle_events();
        update(dt);
        BeginDrawing();
            draw_tile(SpawnPoint, world.spawn, world.screen_offset);
            render();
        EndDrawing();
    }
    
    CloseWindow();

    arena_deinit();
    return 0;
}
