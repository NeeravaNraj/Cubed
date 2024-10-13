#ifndef EDITOR_H
#define EDITOR_H
#include "../tiles.h"
#include "../tilemap.h"
#include "../raylib.h"
#include "../world.h"
#include <unistd.h>

#define BOTTOM_BAR_HEIGHT 100

typedef enum {
    Selection = 1,
    Editing,

    // Special modes
    SetupMovingPlatforms,
} EditorMode;

typedef struct {
    bool opened;
} Menu;

typedef struct {
    Vector2 startPos;
    Vector2 endPos;
    int step;
} MovPltState;

typedef struct {
    World* world;
    Menu menu;

    char selected_variant;
    float camera_speed;

    Tiles selected_tile;
    TileType selected_tile_type;
    Vec(Vector2) selected_tiles;

    Vector2 mouse_pos;
    EditorMode mode;

    MovPltState moving_platform_state;
} EditorState;

void editor_init(EditorState* state, World* world);
void editor_render(EditorState* state);
void editor_handle_events(EditorState* state);
void editor_grid_render(EditorState* state);

void tile_selector_render(EditorState* state);
void tile_selector_handle_events(EditorState* state);

void menu_init(Menu* menu);
void render_menu(EditorState* state);

void movpltstate_init(MovPltState* state);
void movpltstate_process_steps(MovPltState* state, Vector2 position);
#endif // !EDITOR_H
