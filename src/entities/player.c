#include "../inc/raylib.h"
#include "../inc/raymath.h"
#include "../inc/common.h"
#include <stdio.h>
#include "../inc/entities/player.h"
#define PLAYER_COLOR ((Color) { .r = 255, .g = 130, .b = 100, .a = 255 })
#define PLAYER_OUTLINE_COLOR ((Color) { .r = 200, .g = 115, .b = 70, .a = 255 })

#define PLAYER_SIZE 40
#define GRAVITY (0.4f)
#define TERMINAL_VELOCITY 20
#define MAX_JUMPS (2)

typedef enum {
    None,
    Left,
    Right,
    Up,
    Down,
} CollideDirection;

float rotation = 0;
void player_render(Entity* entity, Vector2 offset) {
    Player* player = entity->implementor;
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
    player->entity.velocity.y = minf(velocity - 7, 5);
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

void player_update(Entity* entity, Tilemap* tm, Vector2 offset, float dt) {
    Player* player = entity->implementor;
    
    CollideDirection collide_dir = None;
    Vector2 move_direction = {
        .x = player->movement[0] + entity->velocity.x,
        .y = player->movement[1] + entity->velocity.y,
    };
    entity->position.x += (player->movement[1] - player->movement[0]) * player->move_speed;

    Rectangle player_rect = player_as_rect(entity);
    Tile** tiles_around = tilemap_tiles_around(tm, entity->position);
    for (int i = 0; i < 9; ++i) {
        Tile* tile = tiles_around[i];
        if (!tile) break;
        Rectangle physics_rect = {
            .x = tile->position.x,
            .y = tile->position.y,
            .width = TILE_SIZE,
            .height = TILE_SIZE,
        };

        if (CheckCollisionRecs(player_rect, physics_rect)) {
            float x_overlap = minf(player_rect.x + player_rect.width, physics_rect.x + physics_rect.width) - maxf(player_rect.x, physics_rect.x);

            if (x_overlap >= 0) {
                if (player->movement[1]) {
                    player_rect.x -= x_overlap;
                } else if (player->movement[0]) {
                    player_rect.x += x_overlap;
                }
            }
            entity->position.x = player_rect.x;
        }
    }

    entity->position.y += entity->velocity.y;

    player_rect = player_as_rect(entity);
    tiles_around = tilemap_tiles_around(tm, entity->position);
    for (int i = 0; i < 9; ++i) {
        Tile* tile = tiles_around[i];
        if (!tile) break;
        Rectangle physics_rect = {
            .x = tile->position.x,
            .y = tile->position.y,
            .width = TILE_SIZE,
            .height = TILE_SIZE,
        };

        if (CheckCollisionRecs(player_rect, physics_rect)) {
            float y_overlap = minf(player_rect.y + player_rect.height, physics_rect.y + physics_rect.height) - maxf(player_rect.y, physics_rect.y);

            if (y_overlap >= 0) {
                if (move_direction.y > 0) {
                    player_rect.y -= y_overlap;
                    collide_dir |= Down;
                } else if (move_direction.y < 0) {
                    player_rect.y += y_overlap;
                    collide_dir |= Up;
                }
            }
            entity->position.y = player_rect.y;
        }
    }

    entity->velocity.y = minf(TERMINAL_VELOCITY, entity->velocity.y + GRAVITY);
    if ((collide_dir & Up) || (collide_dir & Down)) {
        entity->velocity.y = 0;
    }
    if (collide_dir & Down) {
        player->jump = 0;
        player->move_speed = 4.5;
    }
}

void player_init(Player* player) {
    player->move_speed = 4.5;
    player->entity.implementor = player;
    player->entity.position.x = player->entity.position.y = 0;
    player->entity.velocity.x = player->entity.velocity.y = 0;
    player->entity.size.x = player->entity.size.y = PLAYER_SIZE;

    player->entity.render = &player_render;
    player->entity.update = &player_update;

    player->movement[0] = player->movement[1] = false;
}
