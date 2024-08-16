#include <raylib.h>
#include <raymath.h>
#include "../inc/common.h"
#include "../inc/entities/player.h"

#define PLAYER_COLOR ((Color) { .r = 255, .g = 130, .b = 100, .a = 255 })
#define PLAYER_OUTLINE_COLOR ((Color) { .r = 200, .g = 115, .b = 70, .a = 255 })

#define PLAYER_SIZE 40
#define GRAVITY (0.4f)
#define MOVEMENT_SPEED 2.5
#define TERMINAL_VELOCITY 20

typedef enum {
    None,
    Left,
    Right,
    Up,
    Down,
} CollideDirection;

void player_render(Entity* entity) {
    int offset = 10;

    Vector2 body;
    Vector2 body_offset;

    body.x = entity->size.x - offset;
    body.y = entity->size.y - offset;
    body_offset.x = entity->position.x + offset / 2; // NOLINT
    body_offset.y = entity->position.y + offset / 2; // NOLINT
    DrawRectangleV(entity->position, entity->size, PLAYER_OUTLINE_COLOR);
    DrawRectangleV(body_offset, body, PLAYER_COLOR);
}

void player_update(Entity* entity, Tilemap* tm, float dt) {
    Player* player = entity->implementor;
    
    CollideDirection collide_dir = None;
    Vector2 move_direction = {
        .x = player->movement[0] + entity->velocity.x,
        .y = player->movement[1] + entity->velocity.y,
    };
    entity->position.x += (player->movement[1] - player->movement[0]) * MOVEMENT_SPEED;
    Rectangle player_rect = {
        .x = entity->position.x,
        .y = entity->position.y,
        .width = PLAYER_SIZE,
        .height = PLAYER_SIZE,
    };

    Tile** tiles_around = tilemap_tiles_around(tm, entity->position);
    for (int i = 0; i < 9; ++i) {
        Tile* tile = tiles_around[i];
        if (!tile) continue;
        Rectangle physics_rect = {
            .x = tile->position.x,
            .y = tile->position.y,
            .width = TILE_SIZE,
            .height = TILE_SIZE,
        };
        if (
            CheckCollisionRecs(player_rect, physics_rect) &&
            physics_rect.y < player_rect.y + player_rect.height - 1
        ) {
            if (move_direction.x > 0) {
                collide_dir |= Right;
                player_rect.x = physics_rect.x - player_rect.width;
            } else if (move_direction.x < 0) {
                collide_dir |= Left;
                player_rect.x = physics_rect.x + physics_rect.width;
            }
            entity->position.x = player_rect.x;
        }
    }
    entity->position.y += entity->velocity.y;
    player_rect.x = entity->position.x;
    player_rect.y = entity->position.y;
    tiles_around = tilemap_tiles_around(tm, entity->position);
    for (int i = 0; i < 9; ++i) {
        Tile* tile = tiles_around[i];
        if (!tile) continue;
        Rectangle physics_rect = {
            .x = tile->position.x,
            .y = tile->position.y,
            .width = TILE_SIZE,
            .height = TILE_SIZE,
        };
        if (CheckCollisionRecs(player_rect, physics_rect)) {
            if (move_direction.y < 0) {
                collide_dir |= Up;
                player_rect.y = physics_rect.y + physics_rect.height;
            } else if (move_direction.y > 0) {
                collide_dir |= Down;
                player_rect.y = physics_rect.y - player_rect.height;
            }
            entity->position.y = player_rect.y;
        }
    }

    entity->velocity.y = minf(TERMINAL_VELOCITY, entity->velocity.y + GRAVITY);
    if ((collide_dir & Up) || (collide_dir & Down)) {
        entity->velocity.y = 0;
    }
}

void init_player(Player* player) {
    player->entity.implementor = player;
    player->entity.position.x = player->entity.position.y = 0;
    player->entity.velocity.x = player->entity.velocity.y = 0;
    player->entity.size.x = player->entity.size.y = PLAYER_SIZE;

    player->entity.render = &player_render;
    player->entity.update = &player_update;

    player->movement[0] = player->movement[1] = false;
}
