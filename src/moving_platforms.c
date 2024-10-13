#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/tiles.h"
#include "inc/common.h"
#include "inc/raylib.h"
#include "inc/raymath.h"
#include "inc/vector.h"
#include "inc/moving_platforms.h"


void moving_platforms_init(MovingPlatforms* plt) {
    plt->platforms = NULL;
    plt->near_platforms = NULL;
    Vec_init(plt->platforms, 8, NULL);
    Vec_init(plt->near_platforms, 8, NULL); // NOLINT
}

size_t moving_platforms_add(MovingPlatforms* plt, Vector2 start, Vector2 end, float speed) {
    size_t length = Vec_length(plt->platforms);
    MovingPlatform platform;
    platform.start_position = start;
    platform.end_position = end;
    platform.speed = speed;
    platform.tiles = NULL;
    platform.tile_start = NULL;
    platform.size = vec2(0, 0);
    Vec_init(platform.tiles, 8, NULL); // NOLINT
    Vec_init(platform.tile_start, 8, NULL);
    Vec_push(plt->platforms, platform);
    
    return length;
}

void moving_platforms_add_tile(MovingPlatforms* plt, size_t i, Tile* tile) {
    MovingPlatform* platform = &plt->platforms[i];

    Vec_push(platform->tiles, tile); // NOLINT
    Vec_push(platform->tile_start, tile->position);

    const size_t total_tiles = Vec_length(platform->tiles);
    float table[total_tiles];
    int counter = 0;

    int x_counts = 0;
    int y_counts = 1;


    for (int i = 0; i < total_tiles; ++i) {
        Tile* tile = platform->tiles[i];
        int x_exists = float_in_array(table, counter, tile->position.x);

        if (x_exists != -1) {
            y_counts++;
        } else {
            table[counter++] = tile->position.x;
            x_counts++;
        }
    }

    platform->size.x = x_counts * TILE_SIZE;
    platform->size.y = y_counts * TILE_SIZE;
}

void moving_platforms_update(MovingPlatforms* plt, float dt) {
    for (int i = 0; i < Vec_length(plt->platforms); ++i) {
        MovingPlatform* platform = &plt->platforms[i];

        bool reached = false;
        float precision =  0.25;
        Vector2 tile_pos_first;
        Vector2 tile_start_first;
        for (int j = 0; j < Vec_length(platform->tiles); ++j) {
            Tile* tile = platform->tiles[j];
            Vector2 tile_start = platform->tile_start[j];
            Vector2 start_diff = Vector2Subtract(tile_start, platform->start_position);
            Vector2 dest = Vector2Add(platform->end_position, start_diff);
            Vector2 end_diff =  Vector2Subtract(dest, tile->position);

            if (j == 0) {
                int x_dir = end_diff.x >= 0 ? 1 : -1;
                int y_dir = end_diff.y > 0 ? 1 : -1;
                float t;

                if (tile_start.x != dest.x) {
                    t = Normalize(tile->position.x + (platform->speed * x_dir), tile_start.x, dest.x) * platform->speed;
                } else {
                    t = Normalize(tile->position.y + (platform->speed * y_dir), tile_start.y, dest.y) * platform->speed;
                }

                if (end_diff.x != 0) {
                    float x_lerp = lerpf(t, tile->position.x, dest.x);
                    platform->velocity.x = x_lerp - tile->position.x;
                    tile->position.x = x_lerp;
                }

                if (end_diff.y != 0) {
                    float y_lerp = lerpf(t, tile->position.y, dest.y);
                    platform->velocity.y = y_lerp - tile->position.y;
                    tile->position.y = lerpf(t, tile->position.y, dest.y);
                }

                tile_pos_first = tile->position;
                tile_start_first = tile_start;
                if (
                    aroundf(precision, tile->position.x, dest.x) &&
                    aroundf(precision, tile->position.y, dest.y)
                ) {
                    reached = true;
                    continue;
                }
            } else {
                Vector2 tile_start_diff = Vector2Subtract(tile_start, tile_start_first);
                tile->position.x = tile_pos_first.x + tile_start_diff.x;
                tile->position.y = tile_pos_first.y + tile_start_diff.y;
            }
        }

        if (reached) {
            Vector2 temp = platform->start_position;
            platform->start_position = platform->end_position;
            platform->end_position = temp;
            platform->velocity.x = 0;
            platform->velocity.y = 0;

            for (int j = 0; j < Vec_length(platform->tiles); ++j) {
                Vector2 old_start = platform->tile_start[j];
                Vector2 start_diff = Vector2Subtract(old_start, platform->end_position);
                platform->tile_start[j] = Vector2Add(platform->start_position, start_diff);
            }
        }
    }
}

void moving_platforms_render(MovingPlatforms* plt, Vector2 offset) {
    for (int i = 0; i < Vec_length(plt->platforms); ++i) {
        MovingPlatform* platform = &plt->platforms[i];
        /* Vector2 mid_point = { */
        /*     .x = platform->start_position.x + ((platform->end_position.x + platform->size.x) - platform->start_position.x) / 2, */
        /*     .y = platform->start_position.y + (platform->end_position.y - platform->start_position.y) / 2, */
        /* }; */
        /* float r = sqrtf(powf(platform->end_position.x - platform->start_position.x, 2) + powf(platform->end_position.y - platform->start_position.y, 2)); */
        /* float extra = maxf(platform->size.x, platform->size.y); */
        /* if (r < extra) { r += extra; } */
        /* Color color = RED; */
        /* color.a = 180; */
        /* DrawCircle(mid_point.x + offset.x, mid_point.y + offset.y, r, color); */
        /* DrawCircle(platform->start_position.x + offset.x, platform->start_position.y + offset.y, 10, ORANGE); */
        /* DrawCircle(mid_point.x + offset.x, mid_point.y + offset.y, 10, WHITE); */
        /* DrawCircle(platform->end_position.x + platform->size.x + offset.x, platform->end_position.y + offset.y, 10, ORANGE); */
        for (int j = 0; j < Vec_length(platform->tiles); ++j) {
            Tile* tile = platform->tiles[j];
            render_tile(tile, offset);
        }
    }
}


void moving_platforms_deinit(MovingPlatforms* plt) {
    for (int i = 0; i < Vec_length(plt->platforms); ++i) {
        MovingPlatform item = plt->platforms[i];
        Vec_free(item.tiles);
    }

    Vec_free(plt->platforms);
    Vec_free(plt->near_platforms);
}

MovingPlatform** moving_platforms_near(MovingPlatforms* plt, Vector2 position) {
    Vec_clear(plt->near_platforms); // NOLINT

    for (int i = 0; i < Vec_length(plt->platforms); ++i) {
        MovingPlatform* platform = &plt->platforms[i];
        Vector2 mid_point = {
            .x = platform->start_position.x + ((platform->end_position.x + platform->size.x) - platform->start_position.x) / 2,
            .y = platform->start_position.y + (platform->end_position.y - platform->start_position.y) / 2,
        };
        float r = sqrtf(powf(platform->end_position.x - platform->start_position.x, 2) + powf(platform->end_position.y - platform->start_position.y, 2));
        float extra = maxf(platform->size.x, platform->size.y);
        if (r < extra) r += extra;
        float dist = sqrtf(powf(position.x - mid_point.x, 2) + powf(position.y - mid_point.y, 2));
        if (dist < r) {
            Vec_push(plt->near_platforms, platform); // NOLINT
        }
    }

    return plt->near_platforms;
}
