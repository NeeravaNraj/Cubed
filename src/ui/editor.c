#define RAYGUI_IMPLEMENTATION
#include <stdio.h>

#include "../inc/level.h"
#include "../inc/raylib.h"
#include "../inc/raygui.h"
#include "../inc/raymath.h"
#include "../inc/ui/editor.h"

#define ROWS (HEIGHT / TILE_SIZE)
#define COLS (WIDTH / TILE_SIZE)
#define ALERT_BOX_WIDTH (300)
#define ALERT_BOX_HEIGHT (150)

extern Font font;
const Rectangle alert_box = {
    .x = ((float)WIDTH / 2) - ((float)ALERT_BOX_WIDTH / 2),
    .y = ((float)HEIGHT / 2) - ((float)ALERT_BOX_HEIGHT / 2),
    .width = ALERT_BOX_WIDTH,
    .height = ALERT_BOX_HEIGHT,
};

const Rectangle save_button = {
    .x = WIDTH - 110,
    .y = HEIGHT - 45,
    .width = 100,
    .height = 35,
};

const Rectangle toggle_selection_button = {
    .x = save_button.x - 40,
    .y = save_button.y - 45,
    .width = 140,
    .height = 35,
};

const Rectangle moving_platform_button = {
    .x = save_button.x - 150,
    .y = HEIGHT - 45,
    .width = 140,
    .height = 35,
};

bool error = false;
char* error_message = "";

void editor_init(EditorState* state, World* world) {
    state->world = world;
    state->mode = Selection;
    state->camera_speed = 1;
    state->selected_tile = 0;
    state->selected_variant = 0;
    state->selected_tile_type = OngridTile;

    state->selected_tiles = NULL;
    Vec_init(state->selected_tiles, 8, NULL);
    menu_init(&state->menu);
    movpltstate_init(&state->moving_platform_state);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
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

void add_selection(EditorState* state, Vector2 position) {
    Vector2 tile_space_coords = to_tile_space(position);
    Tile* tile = tilemap_get_tile(&state->world->tilemap, tile_space_coords);
    if (tile != NULL) {
        bool exists = false;
        for (int i = 0; i < Vec_length(state->selected_tiles); ++i) {
            Vector2 point = state->selected_tiles[i];
            if (point.x == tile_space_coords.x && point.y == tile_space_coords.y) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            Vec_push(state->selected_tiles, tile_space_coords);
        }
    }
}

void remove_selection(EditorState* state, Vector2 position) {
    Vector2 tile_space_coords = to_tile_space(position);
    for (int i = 0; i < Vec_length(state->selected_tiles); ++i) {
        Vector2 point = state->selected_tiles[i];
        if (point.x == tile_space_coords.x && point.y == tile_space_coords.y) {
            Vec_delete(state->selected_tiles, i);
            break;
        }
    }
}

void add_moving_platform(EditorState* state) {
    MovingPlatforms* mplts = &state->world->moving_platforms;
    MovPltState mp_state = state->moving_platform_state;
    size_t index = moving_platforms_add(mplts, mp_state.startPos, mp_state.endPos, 0.07);
    MovingPlatform* platform = &mplts->platforms[index];

    for (int i = 0; i < Vec_length(state->selected_tiles); ++i) {
        Vector2 point = state->selected_tiles[i];
        Tile* tile = tilemap_get_tile(&state->world->tilemap, point);
        assert(tile);
        moving_platforms_add_tile(mplts, index, tile);
        tilemap_remove_tile(&state->world->tilemap, point);
    }

    movpltstate_init(&state->moving_platform_state);
    Vec_clear(state->selected_tiles);
    state->mode = Selection;
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

    if (state->mode == Editing) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && state->mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
            add_tile(state, tile_pos);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && state->mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
            remove_tile(state, tile_pos);
        }
    }

    if (state->mode == Selection && !stop_click_propagation) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && state->mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
            add_selection(state, tile_pos);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && state->mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
            remove_selection(state, tile_pos);
        }
        
        if (IsKeyPressed(KEY_C)) {
            Vec_clear(state->selected_tiles);
        }
    }

    if (state->mode == SetupMovingPlatforms) {
        MovPltState* mp_state = &state->moving_platform_state;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && state->mouse_pos.y < HEIGHT - BOTTOM_BAR_HEIGHT) {
            Vector2 tile_coords = to_tile_space(tile_pos);
            movpltstate_process_steps(mp_state, tile_coords);
        }

        if (mp_state->step == 2) {
            add_moving_platform(state);
        }
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

void render_possible_tile(EditorState* state, Color color) {
    if (state->mouse_pos.y >= HEIGHT - BOTTOM_BAR_HEIGHT) return;

    Camera2D camera = state->world->camera;

    int tile_x = state->mouse_pos.x / TILE_SIZE;
    int tile_y = state->mouse_pos.y / TILE_SIZE;
    color.a = 120;

    DrawRectangle(
        (tile_x * TILE_SIZE),
        (tile_y * TILE_SIZE),
        TILE_SIZE, TILE_SIZE,
        color
    );
}

void render_tile_pos() {
    char buffer[24] = {0};
    int x = mouse_tile_pos.x / TILE_SIZE;
    int y = mouse_tile_pos.y / TILE_SIZE;
    snprintf(buffer, 24, "X=%d, Y=%d", x, y);
    DrawTextEx(font, buffer, vec2(10, 12), 44, 1, BLACK);
}

void render_selection(EditorState* state) {
    Vector2* selections = state->selected_tiles;
    Vector2 offset = state->world->camera.offset;
    size_t length = Vec_length(state->selected_tiles);

    for (size_t i = 0; i < length; ++i) {
        Vector2 point = selections[i];
        Rectangle bounds = {
            .x = point.x + offset.x, .y = point.y + offset.y,
            .width = TILE_SIZE, .height = TILE_SIZE
        };
        DrawRectangleLinesEx(bounds, 2, WHITE);
    }
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
    moving_platforms_render(&world->moving_platforms, world->camera.offset);
    offgrid_render(&world->offgrid_tiles, world->camera.offset);
    render_selection(state);

    if (state->mode == SetupMovingPlatforms && state->moving_platform_state.step == 0) {
        render_possible_tile(state, GREEN);
    }

    if (state->mode == SetupMovingPlatforms && state->moving_platform_state.step == 1) {
        render_possible_tile(state, RED);
    }

    if (state->mode == Selection) {
        if (GuiButton(moving_platform_button, "Make moveable")) {
            if (!Vec_length(state->selected_tiles)) {
                error = true;
                error_message = "No tiles were selected.";
            } else {
                state->mode = SetupMovingPlatforms;
            }
        }
    }

    char* toggle_mode_label = state->mode == Selection ? "Select mode" : "Edit mode";
    if (GuiButton(toggle_selection_button, toggle_mode_label)) {
        state->mode = state->mode == Selection ? Editing : Selection;
    }

    if (GuiButton(save_button, "Save level")) {
        write_level("test_level", world);
        TraceLog(LOG_INFO, "Level saved.");
    }
    render_selected_tile(state);
    if (error) {
        if (GuiMessageBox(alert_box, "Error", error_message, "Ok") >= 0) {
            error = false;
        };
    }
    if (state->menu.opened) { render_menu(state); }

    render_tile_pos();
}
