#include "../../inc/ui/editor.h"
#include <raylib.h>

#define ROWS (HEIGHT / TILE_SIZE)
#define COLS (WIDTH / TILE_SIZE)

void add_tile(EditorState* state, Vector2 position) {
    Tile* tile = arena_alloc(sizeof(Tile));
    tile->kind = state->selected_tile;
    tile->position = position;
    tilemap_add_tile(state->map, tile);
}

void editor_handle_events(EditorState* state) {
    Vector2 mouse_pos = GetMousePosition();
    tile_selector_handle_events(state);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
        add_tile(state, mouse_pos);
    }
}

void editor_init(EditorState* state, Tilemap* map) {
    state->map = map;
    state->selected_tile = -1;
}

void editor_grid_render(EditorState *state) {
    for (int i = 1; i < ROWS + 1; ++i) {
        for (int j = 1; j < COLS + 1; ++j) {
            DrawLine(j * TILE_SIZE, 0, j * TILE_SIZE, HEIGHT, WHITE);
        }
        DrawLine(0, i * TILE_SIZE, WIDTH, i * TILE_SIZE, WHITE);
    }
}

void editor_render(EditorState* state) {
    /* editor_grid_render(state); */
    tile_selector_render(state);
    tilemap_render(state->map);
}
