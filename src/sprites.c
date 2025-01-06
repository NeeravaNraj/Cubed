#include <assert.h>
#include "inc/sprites.h"
#include "inc/common.h"
#include "inc/raylib/raylib.h"

SpriteSheet spritesheet_load(const char* filename, int count, int sprite_width, int sprite_height) {
    SpriteSheet sheet;

    Texture texture = LoadTexture(filename);

    int cols = texture.width / sprite_width;
    int rows = texture.height / sprite_height;

    assert(cols * rows == count);

    sheet.count = count;
    sheet.texture = texture;
    sheet.sprite_width = sprite_width;
    sheet.sprite_height = sprite_height;

    return sheet;
}

Sprite spritesheet_get_sprite(SpriteSheet* sheet, int index) {
    assert(index >= 0 && index < sheet->count);
    Sprite sprite;

    int x = (index % sheet->count) * sheet->sprite_width;
    int y = (index / sheet->count) * sheet->sprite_height;

    sprite.x = x;
    sprite.y = y;
    sprite.w = sheet->sprite_width;
    sprite.h = sheet->sprite_height;
    sprite.texture = &sheet->texture;

    return sprite;
}
