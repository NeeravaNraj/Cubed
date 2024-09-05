#include <unistd.h>
#include <raylib.h>
#include "inc/arena.h"
#include "inc/common.h"
#include "inc/tilemap.h"
#include "inc/tiles.h"
#include "inc/ui/editor.h"
#include "inc/entities/player.h"


#define SKY_COLOR ((Color) { .r = 130, .g = 200, .b = 230, .a = 255 })

struct World {
    Player player;
    Tilemap tilemap;
    EditorState editor_state;
    Vector2 screen_offset;
    bool edit_mode;
} world;

void init() {
    world.edit_mode = !false;
    init_player(&world.player);
    tilemap_init(&world.tilemap);

    if (world.edit_mode) {
        editor_init(&world.editor_state, &world.tilemap);
    }
}

void handle_events() {
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

    if (IsKeyPressed(KEY_P)) {
        world.edit_mode = !world.edit_mode;
    }

    if (world.edit_mode) {
        editor_handle_events(&world.editor_state);
    }
}

void render() {
    if (!world.edit_mode) {
        world.player.entity.render(&world.player.entity);
        tilemap_render(&world.tilemap);
    } else {
        editor_render(&world.editor_state);
    }
}

int main() {
    init();
    arena_init();

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

    InitWindow(WIDTH, HEIGHT, "Cubed");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        ClearBackground(SKY_COLOR);
        float dt = GetFrameTime();
        handle_events();
        world.player.entity.update(&world.player.entity, &world.tilemap, dt);
        BeginDrawing();
            render();
        EndDrawing();
    }
    
    CloseWindow();

    arena_deinit();
    return 0;
}
