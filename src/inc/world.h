#ifndef WORLD_H
#define WORLD_H
#include "common.h"
#include "raylib.h"
#include "offgrid.h"
#include "tilemap.h"
#include "moving_platforms.h"

#define SKY_COLOR ((Color) { .r = 130, .g = 200, .b = 230, .a = 255 })


typedef struct {
    Camera2D camera;

    Tilemap tilemap;
    OffgridTiles offgrid_tiles;
    MovingPlatforms moving_platforms;

    Vector2 spawn;
    Vector2 end;
    bool edit_mode;
} World;

void reset();
Asset* get_asset(Tiles kind);
void load_assets(Assets* assets);
#endif // WORLD_H
