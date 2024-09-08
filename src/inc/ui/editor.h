#ifndef EDITOR_H
#define EDITOR_H
#include "../tiles.h"
#include "../tilemap.h"
#include <raylib.h>

#define BOTTOM_BAR_HEIGHT 100
typedef struct {
    Tiles selected_tile;
    Tilemap* map;
    Vector2* screen_offset;
    float camera_speed;

    Vector2 spawn;
    Vector2 end;
} EditorState;

void tile_selector_render(EditorState* state);
void editor_render(EditorState* state);
void editor_handle_events(EditorState* state);
void tile_selector_handle_events(EditorState* state);
void editor_init(EditorState* state, Tilemap* map, Vector2* screen_offset);
void editor_grid_render(EditorState* state);
#endif // !EDITOR_H
