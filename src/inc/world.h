#ifndef WORLD_H
#define WORLD_H
#include "./entities/player.h"
#include <raylib.h>

#define SKY_COLOR ((Color) { .r = 130, .g = 200, .b = 230, .a = 255 })


typedef struct {
    Player player;
    Tilemap tilemap;
    Vector2 screen_offset;

    Vector2 spawn;
    Vector2 end;
    bool edit_mode;
} World;

typedef struct {
    Tiles selected_tile;
    float camera_speed;
    World* world;
} EditorState;

#endif // WORLD_H