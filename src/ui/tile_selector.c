#include <stddef.h>
#include "../inc/ui.h"
#include "../inc/asset.h"
#include "../inc/editor.h"
#include "../inc/common.h"
#include "../inc/raylib/raylib.h"
#include "../inc/raylib/raygui.h"

Rectangle tileselector_bounds = {0};
Vector2 tileselector_scroll = {0};
Vector2 tileselector_content_size = {140, 320};

void tileselector_init() {
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    tileselector_bounds.x = 0;
    tileselector_bounds.y = height * 0.8;
    tileselector_bounds.width = width;
    tileselector_bounds.height = height * 0.2;
}

void tileselector_content(Vector2 position, Vector2 tileselector_scroll) {
    char* asset_names[3] = {
        "grass_tiles",
        "stone_tiles",
        "small_decor"
    };
    float padding = 16;
    Rectangle src = { .x = 0, .y = 0 };
    Rectangle dest = {
        .x = position.x - TILE_SIZE * 2 + tileselector_scroll.x,
        .y = position.y + padding + tileselector_scroll.y,
        .width = TILE_SIZE * 2,
        .height = TILE_SIZE * 2
    };
    Vector2 origin = {0};
    Vector2 mouse_pos = GetMousePosition();
    for (int i = 0; i < 3; ++i) {
        const Entry* asset_entry = hashmap_get(assets, asset_names[i]);
        SpriteSheet* sheet = asset_entry->value;
        for (int j = 0; j < sheet->count; ++j) {
            Sprite sprite = spritesheet_get_sprite(sheet, j);
            Texture2D tex = *sprite.texture;
            src.x = (float)j * tex.width / sheet->count;
            src.width = (float)tex.width / sheet->count;
            src.height = tex.height;
    
            float x_offset = dest.width + padding;
            float y_offset = 0;
    
            if (dest.x + x_offset + dest.width + padding >= position.x + tileselector_bounds.width) {
                x_offset = 0;
                y_offset = dest.height + padding;
                dest.x = position.x + padding + tileselector_scroll.x;
            }
    
            dest.x += x_offset;
            dest.y += y_offset;
            Rectangle border = dest;
            border.x -= padding / 4;
            border.y -= padding / 4;
            border.height += padding * 0.5;
            border.width += padding * 0.5;
            bool in_tileselector_bounds = CheckCollisionPointRec(mouse_pos, border);
            Color border_color = in_tileselector_bounds ? WHITE : GRAY;
            border_color = editor_state.selected_tile == i && editor_state.selected_variant == j ? WHITE : GRAY;
    
            if (in_tileselector_bounds && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                editor_state.selected_tile = i;
                editor_state.selected_variant = j;
            }
    
            DrawRectangleRec(border, border_color);
            DrawTexturePro(tex, src, dest, origin, 0, WHITE);
        }
    }

    tileselector_content_size.x = tileselector_bounds.width - 20;
    tileselector_content_size.y = ((dest.y + dest.height + padding) - (position.y)) - tileselector_scroll.y;
}

void tileselector_render() {
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    tileselector_bounds.x = SIDEBAR_WIDTH;
    tileselector_bounds.width = width - tileselector_bounds.x;
    tileselector_bounds.height = TILESELECTOR_HEIGHT;
    tileselector_bounds.y = height - tileselector_bounds.height;
    GuiWindow(tileselector_bounds, tileselector_content_size, &tileselector_scroll, tileselector_content);
}

bool tileselector_hovered(Vector2 pos) {
    return CheckCollisionPointRec(pos, tileselector_bounds);
}
