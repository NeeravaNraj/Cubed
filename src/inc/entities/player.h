#ifndef PLAYER_H
#define PLAYER_H

#include "../raylib.h"
#include "../world.h"
#include "../entities.h"

typedef struct Player {
    Entity entity;
    char jump;
    char movement[2];
    float move_speed;
    MovingPlatform* attached_platform;
} Player;

void player_init(Player* player);
void player_jump(Player* player);
void player_render(Player* player, Vector2 offset);
void player_update(Player* player, World* world, Vector2 offset, float dt);
#endif // !PLAYER_H
