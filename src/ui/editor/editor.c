#include "../../inc/ui/editor.h"
#include "../../inc/level.h"
#include "../../inc/raylib.h"
#include "../../inc/raymath.h"
#include <stdio.h>

#define ROWS (HEIGHT / TILE_SIZE)
#define COLS (WIDTH / TILE_SIZE)

/* Button set_spawn = { */
/*     .label = "Set spawn", */
/*     .size = { .x = 120, .y = 30 }, */
/*     .position = { .x = 20, .y = HEIGHT - BOTTOM_BAR_HEIGHT - 50 }, */
/*     .style = { */
/*         .font_size = 16, */
/*         .font_color = WHITE, */ /*         .background_color = { .r = 240, .g = 130, .b = 100, .a = 255 }, */
/*         .border_size = 8, */
/*         .border_color = { .r = 200, .g = 100, .b = 80, .a = 255 }, */
/*     }, */
/* }; */

void editor_init(EditorState* state, World* world) {
    state->world = world;
    state->mode = Editing;
    state->camera_speed = 1;
    state->selected_tile = 0;
    state->selected_variant = 0;
    state->selected_tile_type = OngridTile;
    menu_init(&state->menu);
}

void add_tile(EditorState* state, Vector2 position) {
    Tile* tile = arena_alloc(sizeof(Tile));
    tile->kind = state->selected_tile;
    tile->position = position;
    tile->variant = state->selected_variant;
    if (state->selected_tile_type == OngridTile) {
        tilemap_add_tile(&state->world->tilemap, tile);
    } else {
        offgrid_add_tile(&state->world->offgrid_tiles, tile);
    }
}

void remove_tile(EditorState* state, Vector2 position) {
    bool removed = tilemap_remove_tile(&state->world->tilemap, position);
    if (!removed) {
        offgrid_remove_tile(&state->world->offgrid_tiles, position);
    }
}


Vector2 mouse_tile_pos;
void editor_handle_events(EditorState* state) {
    World* world = state->world;
    state->mouse_pos = GetMousePosition();
    bool stop_click_propagation = false;
    tile_selector_handle_events(state);

    if (IsKeyPressed(KEY_E)) { state->menu.opened ^= 1; }
    if (state->menu.opened) return;
    Vector2 tile_pos = Vector2Subtract(state->mouse_pos, world->camera.offset);
    mouse_tile_pos = tile_pos;

    if (tile_pos.x < 0 && (int)tile_pos.x % TILE_SIZE != 0) {
        int rem = (int)(tile_pos.x) % TILE_SIZE;
        tile_pos.x = tile_pos.x - rem - TILE_SIZE;
    }
    if (tile_pos.y < 0 && (int)tile_pos.y % TILE_SIZE != 0) {
        int rem = (int)(tile_pos.y) % TILE_SIZE;
        tile_pos.y = tile_pos.y - rem - TILE_SIZE;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && state->mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
        add_tile(state, tile_pos);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && state->mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
        remove_tile(state, tile_pos);
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_S)) {
        write_level("test_level", world);
        TraceLog(LOG_INFO, "Level saved.");
        return;
    }

    if (IsKeyDown(KEY_A)) {
        world->camera.offset.x += TILE_SIZE * state->camera_speed;
    }

    if (IsKeyDown(KEY_D)) {
        world->camera.offset.x -= TILE_SIZE * state->camera_speed;
    }

    if (IsKeyDown(KEY_W)) {
        world->camera.offset.y += TILE_SIZE * state->camera_speed;
    }

    if (IsKeyDown(KEY_S)) {
        world->camera.offset.y -= TILE_SIZE * state->camera_speed;
    }

    if (IsKeyPressed(KEY_F)) {
        tilemap_auto_tile(&state->world->tilemap);
    }


    if (IsKeyPressed(KEY_X)) {
        int tile_x = state->mouse_pos.x / TILE_SIZE;
        int tile_y = state->mouse_pos.y / TILE_SIZE;
        world->spawn.x = tile_x * TILE_SIZE - state->world->camera.offset.x;
        world->spawn.y = tile_y * TILE_SIZE - state->world->camera.offset.y;
    }
}

void editor_grid_render(EditorState *state) {
    for (int i = 1; i < ROWS + 1; ++i) {
        for (int j = 1; j < COLS + 1; ++j) {
            DrawLine(j * TILE_SIZE, 0, j * TILE_SIZE, HEIGHT, WHITE);
        }
        DrawLine(0, i * TILE_SIZE, WIDTH, i * TILE_SIZE, WHITE);
    }
}

void render_possible_tile(EditorState* state) {
    if (state->mouse_pos.y >= HEIGHT - BOTTOM_BAR_HEIGHT) return;

    Camera2D camera = state->world->camera;

    int tile_x = state->mouse_pos.x / TILE_SIZE;
    int tile_y = state->mouse_pos.y / TILE_SIZE;
    Color color = WHITE;
    color.a = 120;

    DrawRectangle(
        (tile_x * TILE_SIZE) - 2,
        (tile_y * TILE_SIZE) - 2,
        TILE_SIZE + 4, TILE_SIZE,
        color
    );
}

void render_tile_pos() {
    char buffer[24] = {0};
    int x = mouse_tile_pos.x / TILE_SIZE;
    int y = mouse_tile_pos.y / TILE_SIZE;
    snprintf(buffer, 24, "x=%d, y=%d", x, y);
    DrawText(buffer, 0, 0, 24, BLACK);
}

void render_selected_tile(EditorState* state) {
    Vector2 pos = vec2(10, HEIGHT - TILE_SIZE - 10);
    draw_tile(
        state->selected_tile,
        state->selected_variant,
        pos, vec2(0, 0)
    );
}

void editor_render(EditorState* state) {
    World* world = state->world;
    tilemap_render(&world->tilemap, world->camera.offset);
    offgrid_render(&world->offgrid_tiles, world->camera.offset);
    render_selected_tile(state);

    if (state->menu.opened) { render_menu(state); }

    render_tile_pos();
}
