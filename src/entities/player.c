#include <stdio.h>
#include "../inc/raylib/raylib.h"
#include "../inc/common.h"
#include "../inc/entities/player.h"

#define PLAYER_COLOR ((Color) { .r = 255, .g = 130, .b = 100, .a = 255 })
#define PLAYER_OUTLINE_COLOR ((Color) { .r = 200, .g = 115, .b = 70, .a = 255 })

#define MAX_JUMPS (2)
#define MOVE_SPEED 325
#define GRAVITY (28.0)
#define JUMP_HEIGHT 450.0
#define JUMP_HEIGHT2 300.0
#define TERMINAL_VELOCITY 2000

typedef enum {
    None,
    Left,
    Right,
    Up,
    Down,
    X,
    Y,
} CollideDirection;

Vector2 prevState;

float rotation = 0;
void player_render(Player* player, Vector2 offset, float lag) {
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

    /* float x = lerpf(lag, prevState.x, entity->position.x); */
    /* float y = lerpf(lag, prevState.y, entity->position.y); */

    float x = entity->position.x;
    float y = entity->position.y;
    Rectangle player_rect_out = {
        .x = x + offset.x + center_out.x,
        .y = y + offset.y + center_out.y,
        .width = entity->size.x,
        .height = entity->size.y
    };

    Rectangle player_rect_in = {
        .x = player_rect_out.x,
        .y = player_rect_out.y,
        .width = body.x,
        .height = body.y
    };

    DrawRectanglePro(player_rect_out, center_out, -rotation, PLAYER_OUTLINE_COLOR);
    DrawRectanglePro(player_rect_in, center_in, rotation, PLAYER_COLOR);
}

void player_jump(Player *player) {
    if (player->jump == MAX_JUMPS) return;
    if (player->attached_platform) {
        player->entity.velocity.y = -JUMP_HEIGHT;
    } else if (player->jump == 1) {
        player->entity.velocity.y -= JUMP_HEIGHT2;
    }else {
        player->entity.velocity.y -= JUMP_HEIGHT;
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
        float x_overlap = get_rect_overlap_hor(player_rect, physics_rect);

        if (x_overlap >= 0) {
            if (player->movement[1]) {
                player_rect.x -= x_overlap;
                collide_dir = Right;
            } else if (player->movement[0]) {
                player_rect.x += x_overlap;
                collide_dir = Left;
            } else {
                collide_dir = X;
            }
        }
        entity->position.x = player_rect.x;
    } else {
        float y_overlap = get_rect_overlap_vert(player_rect, physics_rect);
        if (entity->velocity.y > 0) {
            player_rect.y -= y_overlap;
            collide_dir = Down;
        } else if (entity->velocity.y < 0) {
            player_rect.y += y_overlap;
            collide_dir = Up;
        } else {
            collide_dir = Y;
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

CollideDirection handle_moving_platforms_collision(
    Player* player,
    MovingPlatforms* mplts,
    Vector2 move_direction,
    int axis
) {
    Entity* entity = &player->entity;
    CollideDirection collide_dir = None;

    if (axis == 0 && player->attached_platform) {
        entity->position.x += player->attached_platform->velocity.x;
    }

    if (axis == 1 && player->attached_platform) {
        entity->position.y += player->attached_platform->velocity.y;
    }

    Rectangle player_rect = player_as_rect(entity);
    MovingPlatform** nearby_platforms = moving_platforms_near(mplts, entity->position);
    for (int i = 0; i < Vec_length(nearby_platforms); ++i) {
        MovingPlatform* platform = nearby_platforms[i];
        if (axis == 1) {
            for (int j = 0; j < Vec_length(platform->tiles); ++j) {
                Tile* tile = platform->tiles[j];
                if (!tile) continue;
                Rectangle physics_rect = rect_from_tile(tile);
                if (CheckCollisionRecs(player_rect, physics_rect)) {
                    if (player->attached_platform) {
                        entity->position.y = tile->position.y - PLAYER_SIZE;
                        collide_dir = Down;
                    } else {
                        collide_dir = handle_axis_collision(player, player_rect, physics_rect, axis);
                    }
                    break;
                }
            }
        } else {
           for (int j = 0; j < Vec_length(platform->tiles); ++j) {
                Tile* tile = platform->tiles[j];
                if (!tile) continue;
                Rectangle physics_rect = rect_from_tile(tile);
                float player_center = player_rect.y + player_rect.height / 2;
                if (CheckCollisionRecs(player_rect, physics_rect)) {
                    if (player_center > physics_rect.y) {
                        float x_overlap = get_rect_overlap_hor(player_rect, physics_rect);
                        float platform_move_direction = platform->velocity.x >= 0 ? 1 : -1;
                        if (x_overlap >= 0) {
                            if (player->movement[1]) {
                                player_rect.x -= x_overlap;
                                collide_dir = Right;
                            } else if (player->movement[0]) {
                                player_rect.x += x_overlap;
                                collide_dir = Left;
                            } else if (!player->attached_platform) {
                                player_rect.x += (x_overlap + 0.2) * platform_move_direction;
                                collide_dir = platform_move_direction == 1 ? Left : Right;
                            }
                        }
                        entity->position.x = player_rect.x;
                        break;
                    }
                }
            }
        }

        if ((collide_dir == Up) || (collide_dir == Down)) {
            entity->velocity.y = 0;
        }

        if (collide_dir == Down) {
            player->jump = 0;
            player->move_speed = MOVE_SPEED;
            player->attached_platform = platform;
        }
    }

    return collide_dir;
}

void player_update(Player* player, World* world, Vector2 offset, float dt) {
    Entity* entity = &player->entity;
    Tilemap* tm = &world->tilemap;
    MovingPlatforms* mplts = &world->moving_platforms;
    CollideDirection collide_dir1 = None;
    CollideDirection collide_dir2 = None;
    Vector2 move_direction = {
        .x = player->movement[0] + entity->velocity.x,
        .y = player->movement[1] + entity->velocity.y,
    };

    prevState = entity->position;
    entity->velocity.x = (player->movement[1] - player->movement[0]) * player->move_speed * dt;
    entity->position.x += entity->velocity.x;
    collide_dir1 = handle_tilemap_collision(player, tm, move_direction, 0);
    collide_dir2 = handle_moving_platforms_collision(player, mplts, move_direction, 0);

    if (collide_dir1 == X && (collide_dir2 == Right || collide_dir2 == Left)) {
        reset();
        return;
    }

    entity->position.y += entity->velocity.y * dt;

    collide_dir1 = handle_tilemap_collision(player, tm, move_direction, 1);
    handle_moving_platforms_collision(player, mplts, move_direction, 1);

    entity->velocity.y = minf(TERMINAL_VELOCITY, entity->velocity.y + GRAVITY);
    if ((collide_dir1 == Up) || (collide_dir1 == Down)) {
        entity->velocity.y = 0;
    }

    if (collide_dir1 == Down) {
        player->jump = 0;
        player->move_speed = MOVE_SPEED;
        player->attached_platform = NULL;
    }

    if (player->jump == MAX_JUMPS) {
        float direction = player->movement[0] ? -1 : 1;
        rotation += PI * 4 *  direction;
    } else {
        rotation = 0;
    }
}

void player_handle_inputs(Player* player) {
    if (IsKeyDown(KEY_A)) {
        player->movement[0] = true;
    } else {
        player->movement[0] = false;
    }

    if (IsKeyDown(KEY_D)) {
        player->movement[1] = true;
    } else {
        player->movement[1] = false;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        player_jump(player);
    }
}

void player_init(Player* player) {
    player->move_speed = MOVE_SPEED;
    player->attached_platform = NULL;
    player->entity.implementor = player;
    player->entity.position.x = player->entity.position.y = 0;
    player->entity.velocity.x = player->entity.velocity.y = 0;
    player->entity.size.x = player->entity.size.y = PLAYER_SIZE;

    player->movement[0] = player->movement[1] = false;
}
