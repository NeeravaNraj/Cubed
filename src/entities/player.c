#include "../inc/raylib.h"
#include "../inc/common.h"
#include <stdio.h>
#include "../inc/entities/player.h"
#define PLAYER_COLOR ((Color) { .r = 255, .g = 130, .b = 100, .a = 255 })
#define PLAYER_OUTLINE_COLOR ((Color) { .r = 200, .g = 115, .b = 70, .a = 255 })

#define PLAYER_SIZE 40
#define GRAVITY (0.4f)
#define TERMINAL_VELOCITY 20
#define MAX_JUMPS (2)
#define JUMP_HEIGHT 7.0

typedef enum {
    None,
    Left,
    Right,
    Up,
    Down,
} CollideDirection;

float rotation = 0;
void player_render(Player* player, Vector2 offset) {
    Entity* entity = &player->entity;
    int border_offset = 10;

    Vector2 body;
    Vector2 body_offset;

    body.x = entity->size.x - border_offset;
    body.y = entity->size.y - border_offset;
    body_offset.x = entity->position.x + border_offset / 2; // NOLINT
    body_offset.y = entity->position.y + border_offset / 2; // NOLINT
    Vector2 center_in = {
        .x = body.x / 2,
        .y = body.y / 2,
    };

    Vector2 center_out = {
        .x = (body.x + border_offset) / 2,
        .y = (body.y + border_offset) / 2,
    };

    Rectangle player_rect_out = {
        .x = entity->position.x + offset.x + center_out.x,
        .y = entity->position.y + offset.y + center_out.y,
        .width = entity->size.x,
        .height = entity->size.y
    };

    Rectangle player_rect_in = {
        .x = player_rect_out.x,
        .y = player_rect_out.y,
        .width = body.x,
        .height = body.y
    };

    if (player->jump == MAX_JUMPS) {
        if (player->movement[0]) {
            rotation -= PI * 4;
        } else {
            rotation += PI * 4;
        }
    } else {
        rotation = 0;
    }

    DrawRectanglePro(player_rect_out, center_out, -rotation, PLAYER_OUTLINE_COLOR);
    DrawRectanglePro(player_rect_in, center_in, rotation, PLAYER_COLOR);
}

void player_jump(Player *player) {
    if (player->jump == MAX_JUMPS) return;
    float velocity = player->entity.velocity.y;
    if (player->attached_platform) {
        player->entity.velocity.y = -JUMP_HEIGHT;
    } else {
        player->entity.velocity.y = velocity - JUMP_HEIGHT;
    }
    player->attached_platform = NULL;
    player->jump += 1;
}

Rectangle player_as_rect(Entity* entity) {
    Rectangle player_rect = {
        .x = entity->position.x,
        .y = entity->position.y,
        .width = PLAYER_SIZE,
        .height = PLAYER_SIZE,
    };

    return player_rect;
}

CollideDirection handle_axis_collision(
    Player* player,
    Rectangle player_rect,
    Rectangle physics_rect,
    int axis
) {
    Entity* entity = &player->entity;
    CollideDirection collide_dir = None;
    if (axis == 0) {
        float x_overlap = minf(player_rect.x + player_rect.width, physics_rect.x + physics_rect.width) - maxf(player_rect.x, physics_rect.x);

        if (x_overlap >= 0) {
            if (player->movement[1]) {
                player_rect.x -= x_overlap;
            } else if (player->movement[0]) {
                player_rect.x += x_overlap;
            }
        }
        entity->position.x = player_rect.x;
    } else {
        float y_overlap = minf(player_rect.y + player_rect.height, physics_rect.y + physics_rect.height) - maxf(player_rect.y, physics_rect.y);
        if (entity->velocity.y > 0) {
            player_rect.y -= y_overlap;
            collide_dir |= Down;
        } else if (entity->velocity.y < 0) {
            player_rect.y += y_overlap;
            collide_dir |= Up;
        }
        entity->position.y = player_rect.y;
    }

    return collide_dir;
}

CollideDirection handle_tilemap_collision(Player* player, Tilemap* tm, Vector2 move_direction, int axis) {
    Entity* entity = &player->entity;
    CollideDirection collide_dir = None;

    if (axis == 0) {
        Rectangle player_rect = player_as_rect(entity);
        Tile** tiles_around = tilemap_tiles_around(tm, entity->position);
        for (int i = 0; i < 9; ++i) {
            Tile* tile = tiles_around[i];
            if (!tile) break;
            Rectangle physics_rect = rect_from_tile(tile);
            if (CheckCollisionRecs(player_rect, physics_rect)) {
                collide_dir = handle_axis_collision(player, player_rect, physics_rect, axis);
            }
        }
    } else {
        Rectangle player_rect = player_as_rect(entity);
        Tile** tiles_around = tilemap_tiles_around(tm, entity->position);
        for (int i = 0; i < 9; ++i) {
            Tile* tile = tiles_around[i];
            if (!tile) break;
            Rectangle physics_rect = rect_from_tile(tile);

            if (CheckCollisionRecs(player_rect, physics_rect)) {
                collide_dir = handle_axis_collision(player, player_rect, physics_rect, axis);
            }
        }
    }

    return collide_dir;
}

void handle_moving_platforms_collision(
    Player* player,
    MovingPlatforms* mplts,
    Vector2 move_direction,
    int axis
) {
    Entity* entity = &player->entity;
    CollideDirection collide_dir = None;
    Rectangle player_rect = player_as_rect(entity);

    MovingPlatform** nearby_platforms = moving_platforms_near(mplts, entity->position);
    for (int i = 0; i < Vec_length(nearby_platforms); ++i) {
        MovingPlatform* platform = nearby_platforms[i];
        if (axis == 1) {
            for (int j = 0; j < Vec_length(platform->tiles); ++j) {
                Tile* tile = platform->tiles[j];
                if (!tile) break;
                Rectangle physics_rect = rect_from_tile(tile);
                if (CheckCollisionRecs(player_rect, physics_rect)) {
                    if (player->attached_platform) {
                        entity->position.y = tile->position.y - player_rect.height;
                    } else {
                        collide_dir = handle_axis_collision(player, player_rect, physics_rect, axis);
                    }
                }
            }
        } else {
           for (int j = 0; j < Vec_length(platform->tiles); ++j) {
                Tile* tile = platform->tiles[j];
                if (!tile) break;
                Rectangle physics_rect = rect_from_tile(tile);
                if (CheckCollisionRecs(player_rect, physics_rect)) {
                    if (!player->attached_platform) {
                        handle_axis_collision(player, player_rect, physics_rect, axis);
                    }
                }
            }
        }

        if (axis == 0 && platform == player->attached_platform) {
            entity->position.x += platform->velocity.x;
        }
        if ((collide_dir & Up) || (collide_dir & Down)) {
            entity->velocity.y = 0;
        }

        if (collide_dir & Down) {
            player->jump = 0;
            player->move_speed = 4.5;
            player->attached_platform = platform;
        }
    }
}

void player_update(Player* player, World* world, Vector2 offset, float dt) {
    Entity* entity = &player->entity;
    Tilemap* tm = &world->tilemap;
    MovingPlatforms* mplts = &world->moving_platforms;
    CollideDirection collide_dir = None;
    Vector2 move_direction = {
        .x = player->movement[0] + entity->velocity.x,
        .y = player->movement[1] + entity->velocity.y,
    };

    entity->velocity.x = (player->movement[1] - player->movement[0]) * player->move_speed;
    entity->position.x += entity->velocity.x;
    handle_tilemap_collision(player, tm, move_direction, 0);
    handle_moving_platforms_collision(player, mplts, move_direction, 0);
    entity->position.y += entity->velocity.y;
    collide_dir = handle_tilemap_collision(player, tm, move_direction, 1);
    handle_moving_platforms_collision(player, mplts, move_direction, 1);

    entity->velocity.y = minf(TERMINAL_VELOCITY, entity->velocity.y + GRAVITY);
    if ((collide_dir & Up) || (collide_dir & Down)) {
        entity->velocity.y = 0;
    }

    if (collide_dir & Down) {
        player->jump = 0;
        player->move_speed = 4.5;
        player->attached_platform = NULL;
    }
}

void player_init(Player* player) {
    player->move_speed = 4.5;
    player->entity.implementor = player;
    player->entity.position.x = player->entity.position.y = 0;
    player->entity.velocity.x = player->entity.velocity.y = 0;
    player->entity.size.x = player->entity.size.y = PLAYER_SIZE;
    player->attached_platform = NULL;

    player->movement[0] = player->movement[1] = false;
}
