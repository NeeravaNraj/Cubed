#include "raylib.h"
#include "../../inc/common.h"
#include "../../inc/tiles.h"

#define BOTTOM_BAR_HEIGHT 100
#define THUMBNAIL_PADDING 12
#define PADDING 10
#define ITEM_BOX_SIZE (BOTTOM_BAR_HEIGHT - PADDING * 2)

void tile_selector_handle_events() {
    
}

bool mouse_in_item_box(Vector2 mouse_pos, Vector2 box_pos) {
    if (
        mouse_pos.x >= box_pos.x &&
        mouse_pos.x < box_pos.x + ITEM_BOX_SIZE &&
        mouse_pos.y >= box_pos.y &&
        mouse_pos.y < box_pos.y + ITEM_BOX_SIZE
    ) return true;

    return false;
}

void tile_selector_render() {
    Color item_box_color = { .r = 200, .g = 200, .b = 200, .a = 255 };
    Color item_box_color_hover = { .r = 240, .g = 240, .b = 240, .a = 255 };
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
        Vector2 box_pos = (Vector2) {
            .x = x,
            .y = y,
        };
        DrawRectangleV(
            box_pos,
            (Vector2) { .x = ITEM_BOX_SIZE, .y = ITEM_BOX_SIZE, },
            mouse_in_item_box(mouse_pos, box_pos) ? item_box_color_hover : item_box_color
        );
        draw_tile(i, (Vector2) {
            .x = x + THUMBNAIL_PADDING,
            .y = y + THUMBNAIL_PADDING,
        });
    }
}
