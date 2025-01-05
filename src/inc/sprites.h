#ifndef SPRITES_H
#define SPRITES_H

#include "./raylib/raylib.h"

typedef struct {
    int count;
    int sprite_width;
    int sprite_height;
    Texture texture;
} SpriteSheet;

typedef struct {
    Texture* texture;
    int x, y;
    int w, h;
} Sprite;


SpriteSheet spritesheet_load(const char* filename, int count, int sprite_width, int sprite_height);
Sprite spritesheet_get_sprite(SpriteSheet* sheet, int index);
#endif // !SPRITES_H
