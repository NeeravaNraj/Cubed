#ifndef MOVING_PLATFORMS_H
#define MOVING_PLATFORMS_H

#include "raylib.h"
#include "tiles.h"
#include "vector.h"

typedef struct {
    Vector2 start_position;
    Vector2 end_position;
    Vector2 size;
    Vector2 velocity;
    Vec(Tile*) tiles;
    Vec(Vector2) tile_start;
    float speed;
} MovingPlatform;

typedef struct {
    Vec(MovingPlatform) platforms;
    Vec(MovingPlatform*) near_platforms;
} MovingPlatforms;

void moving_platforms_init(MovingPlatforms* plt);
size_t moving_platforms_add(MovingPlatforms* plt, Vector2 start, Vector2 end, float speed);
void moving_platforms_add_tile(MovingPlatforms* plt, size_t i, Tile* tile);
void moving_platforms_update(MovingPlatforms* plt, float dt);
void moving_platforms_render(MovingPlatforms* plt, Vector2 offset);
MovingPlatform** moving_platforms_near(MovingPlatforms* plt, Vector2 position);
void moving_platforms_deinit(MovingPlatforms* plt);
#endif // !OFFGRID_H
