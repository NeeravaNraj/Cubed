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

void player_update(Entity* entity, float dt) {
    Player* player = entity->implementor;
    entity->position.y += entity->velocity.y;

    entity->position.x += (player->movement[1] - player->movement[0]) * MOVEMENT_SPEED;
    entity->velocity.y = minf(TERMINAL_VELOCITY, entity->velocity.y + GRAVITY);
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
