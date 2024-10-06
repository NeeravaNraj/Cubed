#include "../../inc/common.h"
#include "../../inc/ui/editor.h"
#include <unistd.h>
#include <assert.h>

#define MENU_WIDTH (WIDTH * 0.6)
#define MENU_HEIGHT (HEIGHT * 0.7)
#define ITEM_SIZE (32)
#define COLS (MENU_WIDTH / ITEM_SIZE)
#define PADDING_VER 32
#define PADDING_HOR 24
#define BOX_PADDING 8

void menu_init(Menu* menu) {
    menu->opened = false;
}

void render_item(EditorState* state, Asset asset, char variant, int i, Vector2 offset) {
    float menu_x = ((float)WIDTH / 2) - (MENU_WIDTH / 2);
    float menu_y = ((float)HEIGHT / 2) - (MENU_HEIGHT / 2);

    int x = (i % (int)MENU_WIDTH);
    float dest_x = x * ITEM_SIZE + menu_x + PADDING_HOR * (x + 1) + (PADDING_VER - PADDING_HOR);
    float dest_y = (i / MENU_WIDTH) * ITEM_SIZE + menu_y + PADDING_VER;

    if (dest_x + ITEM_SIZE >= MENU_WIDTH - PADDING_HOR) {
        dest_y += ITEM_SIZE;
    }

    dest_y += offset.y;
    dest_x += offset.x;

    Texture2D texture = asset.textures[0];
    Rectangle src = {
        .x = TEXTURE_SIZE * variant, .y = 0,
        .width = (float)texture.width / asset.len,
        .height = (float)texture.height
    };
    Rectangle dest = {
        .x = dest_x,
        .y = dest_y,
        .width = ITEM_SIZE,
        .height = ITEM_SIZE
    };
    Rectangle bounding_rect = {
        .x = dest_x - BOX_PADDING,
        .y = dest_y - BOX_PADDING,
        .width = ITEM_SIZE + BOX_PADDING * 2,
        .height = ITEM_SIZE + BOX_PADDING * 2,
    };

    Vector2 origin = { .x = 0, .y = 0 };
    Color bounding_rect_color = WHITE;

    if (CheckCollisionPointRec(state->mouse_pos, bounding_rect)) {
        bounding_rect_color = (Color){
            .r = 240, .g = 240, .b = 0, .a = 120
        };

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (asset.asset_flags & OngridTile) {
                state->selected_tile = asset.kind;
                state->selected_variant = 0;
                state->selected_tile_type = OngridTile;
            } else if (asset.asset_flags & OffgridTile) {
                state->selected_tile = asset.kind;
                state->selected_variant = variant;
                state->selected_tile_type = OffgridTile;
            }
        }
    }

    DrawRectangle(
        bounding_rect.x, bounding_rect.y,
        bounding_rect.width, bounding_rect.height,
        bounding_rect_color
    );
    DrawTexturePro(texture, src, dest, origin, 0, WHITE);
}

void render_items(EditorState* state) {
    size_t tile_assets_len = sizeof(assets.tile_assets) / sizeof(Asset);
    Asset* tile_asset_list = (Asset*)&assets.tile_assets;
    Vector2 offset = vec2(0, 0);

    for (int i = 0; i < tile_assets_len; ++i) {
        Asset asset = tile_asset_list[i];
        render_item(state, asset, 0, i, offset);
    }

    offset.y += ITEM_SIZE * 2;
    for (int i = 0; i < assets.decor_assets.small_decor.len; ++i) {
        Asset asset = assets.decor_assets.small_decor;
        render_item(state, asset, i, i, offset);
    }
}

void render_menu(EditorState* state) {
    Texture2D menu = assets.menu.textures[0];
    Rectangle src = {
        .x = 0, .y = 0,
        .width = menu.width,
        .height = menu.height
    };
    Rectangle dest = {
        .x = ((float)WIDTH / 2) - (MENU_WIDTH / 2),
        .y = ((float)HEIGHT / 2) - (MENU_HEIGHT / 2),
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT
    };

    Vector2 origin = { .x = 0, .y = 0 };

    DrawTexturePro(menu, src, dest, origin, 0, WHITE);
    render_items(state);
}
