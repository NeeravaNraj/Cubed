#include "../../inc/ui/editor.h"
#include "../../inc/level.h"
#include "../../inc/ui/button.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define ROWS (HEIGHT / TILE_SIZE)
#define COLS (WIDTH / TILE_SIZE)

/* Button set_spawn = { */
/*     .font_size = 16, */
/*     .label = "Set spawn", */
/*     .size = { .x = 120, .y = 30 }, */
/*     .position = { .x = 20, .y = HEIGHT - BOTTOM_BAR_HEIGHT - 50 }, */
/*     .font_color = WHITE, */
/*     .background_color = { .r = 240, .g = 130, .b = 100, .a = 255 }, */
/*     .border_size = 8, */
/*     .border_color = { .r = 200, .g = 100, .b = 80, .a = 255 }, */
/* }; */

Vector2 mouse_pos;
void add_tile(EditorState* state, Vector2 position) {
    Tile* tile = arena_alloc(sizeof(Tile));
    tile->kind = state->selected_tile;
    tile->position = position;
    tilemap_add_tile(&state->world->tilemap, tile);
}


void editor_handle_events(EditorState* state) {
    World* world = state->world;
    mouse_pos = GetMousePosition();
    tile_selector_handle_events(state);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
        Vector2 tile_pos = Vector2Subtract(mouse_pos, world->screen_offset);
        add_tile(state, tile_pos);
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
        write_level("test_level", world);
        TraceLog(LOG_INFO, "Level saved.");
        return;
    }

    if (IsKeyDown(KEY_A)) {
        world->screen_offset.x += 1 * state->camera_speed;
    }

    if (IsKeyDown(KEY_D)) {
        world->screen_offset.x -= 1 * state->camera_speed;
    }

    if (IsKeyDown(KEY_W)) {
        world->screen_offset.y += 1 * state->camera_speed;
    }

    if (IsKeyDown(KEY_S)) {
        world->screen_offset.y -= 1 * state->camera_speed;
    }

    /* printf("mouse_pos: %f, %f\n", mouse_pos.x, mouse_pos.y); */
    if (IsKeyPressed(KEY_X)) {
        world->spawn.x = (mouse_pos.x / TILE_SIZE) * TILE_SIZE;
        world->spawn.y = (mouse_pos.y / TILE_SIZE) * TILE_SIZE;
    }
}

void editor_init(EditorState* state, World* world) {
    state->world = world;
    state->camera_speed = 5;
    state->selected_tile = 0;
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
    if (mouse_pos.y >= HEIGHT - BOTTOM_BAR_HEIGHT) return;
    int tile_x = mouse_pos.x / TILE_SIZE;
    int tile_y = mouse_pos.y / TILE_SIZE;
    Color color = WHITE;
    color.a = 120;

    DrawRectangle(
        (tile_x * TILE_SIZE) - 2,
        (tile_y * TILE_SIZE) - 2,
        TILE_SIZE + 4, TILE_SIZE,
        color
    );
}

void editor_render(EditorState* state) {
    /* editor_grid_render(state); */
    World* world = state->world;
    tilemap_render(&world->tilemap, world->screen_offset);
    render_possible_tile(state);
    tile_selector_render(state);
}
