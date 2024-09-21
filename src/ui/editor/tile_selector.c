#include "../../inc/raylib.h"
#include "../../inc/common.h"
#include "../../inc/tiles.h"
#include "../../inc/ui/editor.h"
#include <stdint.h>
#include <unistd.h>

#define PADDING 10
#define ITEM_BOX_SIZE (BOTTOM_BAR_HEIGHT - PADDING * 2)
/* #define THUMBNAIL_PADDING (ITEM_BOX_SIZE / TILE_SIZE + PADDING) */
#define ITEM_BOX_BG ((Color) { .r = 200, .g = 200, .b = 200, .a = 255 })
#define ITEM_BOX_BG_HOVER ((Color) { .r = 220, .g = 220, .b = 220, .a = 255 })
#define ITEM_BOX_BG_SELECTED ((Color) { .r = 180, .g = 240, .b = 180, .a = 255 })

const float THUMBNAIL_PADDING = (float)ITEM_BOX_SIZE - (float)TILE_SIZE;


Vector2 itemboxes[16];
size_t item_count = 0;


bool mouse_in_item_box(Vector2 mouse_pos, Vector2 box_pos) {
    if (
        mouse_pos.x >= box_pos.x &&
        mouse_pos.x < box_pos.x + ITEM_BOX_SIZE &&
        mouse_pos.y >= box_pos.y &&
        mouse_pos.y < box_pos.y + ITEM_BOX_SIZE
    ) return true;

    return false;
}


void tile_selector_handle_events(EditorState* state) {
    Vector2 mouse_pos = GetMousePosition();

    for (int i = 0; i < item_count; ++i) {
        if (
            mouse_in_item_box(mouse_pos, itemboxes[i]) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        ) {
            state->selected_variant = i;
            break;
        }
    }
}


void tile_selector_render(EditorState* state) {
    Vector2 mouse_pos = GetMousePosition();
    // Draw bottom bar
    DrawRectangle(
        0, HEIGHT - BOTTOM_BAR_HEIGHT,
        WIDTH, BOTTOM_BAR_HEIGHT,
        (Color){ .r = 125, .g = 125, .b = 125, .a = 125 }
    );

    Asset* asset =  get_asset(state->selected_tile);
    item_count = asset->len;
    for (char i = 0; i < asset->len; ++i) {
        int x = i * ITEM_BOX_SIZE + PADDING * (i + 1);
        int y = HEIGHT - BOTTOM_BAR_HEIGHT + PADDING;

        Vector2 box_pos = { .x = x, .y = y, };
        itemboxes[i] = box_pos;

        Color color = mouse_in_item_box(mouse_pos, box_pos) ? ITEM_BOX_BG_HOVER : ITEM_BOX_BG;
        DrawRectangleV(
            box_pos,
            (Vector2) { .x = ITEM_BOX_SIZE, .y = ITEM_BOX_SIZE, },
            state->selected_variant == i ? ITEM_BOX_BG_SELECTED : color
        );

        draw_tile(
            state->selected_tile, i,
            (Vector2) { .x = x + THUMBNAIL_PADDING / 2, .y = y + THUMBNAIL_PADDING, },
            (Vector2) {.x = 0, .y = 0}
        );
    }
}
