#include "raylib.h"
#include "../../inc/common.h"
#include "../../inc/tiles.h"
#include "../../inc/ui/editor.h"

#define THUMBNAIL_PADDING 12
#define PADDING 10
#define ITEM_BOX_SIZE (BOTTOM_BAR_HEIGHT - PADDING * 2)
#define ITEM_BOX_BG ((Color) { .r = 200, .g = 200, .b = 200, .a = 255 })
#define ITEM_BOX_BG_HOVER ((Color) { .r = 220, .g = 220, .b = 220, .a = 255 })
#define ITEM_BOX_BG_SELECTED ((Color) { .r = 180, .g = 240, .b = 180, .a = 255 })

#define NUMBER_BOXES (EndTile)

Vector2 itemboxes[NUMBER_BOXES];


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

    for (int i = 0; i < EndTile; ++i) {
        if (
            mouse_in_item_box(mouse_pos, itemboxes[i]) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        ) {
            state->selected_tile = i;
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

    for (int i = 0; i < EndTile; ++i) {
        int x = i * ITEM_BOX_SIZE + PADDING * (i + 1);
        int y = HEIGHT - BOTTOM_BAR_HEIGHT + PADDING;
        Vector2 box_pos = { .x = x, .y = y, };
        itemboxes[i] = box_pos;
        Color color = mouse_in_item_box(mouse_pos, box_pos) ? ITEM_BOX_BG_HOVER : ITEM_BOX_BG;
        DrawRectangleV(
            box_pos,
            (Vector2) { .x = ITEM_BOX_SIZE, .y = ITEM_BOX_SIZE, },
            state->selected_tile == i ? ITEM_BOX_BG_SELECTED : color
        );
        draw_tile(i, (Vector2) {
            .x = x + THUMBNAIL_PADDING,
            .y = y + THUMBNAIL_PADDING,
        });
    }
}
