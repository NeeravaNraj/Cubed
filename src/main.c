#include <unistd.h>
#include <raylib.h>

#include "inc/arena.h"
#include "inc/common.h"
#include "inc/entities/player.h"
#include "inc/tiles.h"


#define SKY_COLOR ((Color) { .r = 130, .g = 200, .b = 230, .a = 255 })

struct World {
    Player player;

} world;

void init() {
    init_player(&world.player);
}

int main() {
    init();
    arena_init();

    Tile tile = {
        .position = {
            .x = 200,
            .y = 600,
        },
        .kind = Platform,
    };
    Tile tile2 = {
        .position = {
            .x = 400,
            .y = 550,
        },
        .kind = GrassPlatform,
    };
    world.player.entity.position.x = 250;

    InitWindow(WIDTH, HEIGHT, "Cubed");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        ClearBackground(SKY_COLOR);
        float dt = GetFrameTime();

        switch (GetKeyPressed()) {
            case KEY_SPACE:
                world.player.entity.velocity.y -= 7;
                break;
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


        world.player.entity.update(&world.player.entity, dt);
        BeginDrawing();
            render_tile(&tile);
            render_tile(&tile2);
            world.player.entity.render(&world.player.entity);
        EndDrawing();
    }
    
    CloseWindow();

    arena_deinit();
    return 0;
}
