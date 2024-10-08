#ifndef EDITOR_H
#define EDITOR_H
#include "../tiles.h"
#include "../tilemap.h"
#include "../raylib.h"
#include "../world.h"
#include <unistd.h>

#define BOTTOM_BAR_HEIGHT 100

typedef enum EditorMode {
    Selection,
    Editing,
} EditorMode;

typedef struct {
    bool opened;
} Menu;

typedef struct {
    World* world;
    Menu menu;

    char selected_variant;
    float camera_speed;

    Tiles selected_tile;
    TileType selected_tile_type;
    
    Vector2 mouse_pos;
    Vector2* itemboxes;

    EditorMode mode;
} EditorState;

void editor_init(EditorState* state, World* world);
void editor_render(EditorState* state);
void editor_handle_events(EditorState* state);
void editor_grid_render(EditorState* state);

void tile_selector_render(EditorState* state);
void tile_selector_handle_events(EditorState* state);

void menu_init(Menu* menu);
void render_menu(EditorState* state);
#endif // !EDITOR_H
