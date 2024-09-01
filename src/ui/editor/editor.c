#include "../../inc/ui/editor.h"

void editor_handle_events() {
    tile_selector_handle_events();
}

void editor_init(EditorState* state, Tilemap* map) {
    state->map = map;
    state->selected_tile = -1;
}

void editor_render() {
    tile_selector_render();
}
