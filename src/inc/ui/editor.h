#ifndef EDITOR_H
#define EDITOR_H
#include "../tiles.h"
#include "../tilemap.h"
#include "../raylib.h"
#include "../world.h"

#define BOTTOM_BAR_HEIGHT 100

void editor_init(EditorState* state, World* world);
void tile_selector_render(EditorState* state);
void editor_render(EditorState* state);
void editor_handle_events(EditorState* state);
void tile_selector_handle_events(EditorState* state);
void editor_grid_render(EditorState* state);
#endif // !EDITOR_H
