#ifndef EDITOR_H
#define EDITOR_H
#include "../tiles.h"
#include "../tilemap.h"

typedef struct {
    int selected_tile;
    Tilemap* map;
} EditorState;

void tile_selector_render();
void editor_render();
void editor_handle_events();
void tile_selector_handle_events();
void editor_init(EditorState* state, Tilemap* map);
#endif // !EDITOR_H
