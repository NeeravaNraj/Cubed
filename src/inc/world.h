#ifndef WORLD_H
#define WORLD_H
#include "common.h"
#include "offgrid.h"
#include "raylib.h"
#include "entities/player.h"

#define SKY_COLOR ((Color) { .r = 130, .g = 200, .b = 230, .a = 255 })


typedef struct {
    Player player;
    Camera2D camera;

    Tilemap tilemap;
    OffgridTiles offgrid_tiles;

    Vector2 spawn;
    Vector2 end;
    bool edit_mode;
} World;

void load_assets(Assets* assets);
Asset* get_asset(Tiles kind);
#endif // WORLD_H
