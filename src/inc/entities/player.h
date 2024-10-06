#ifndef PLAYER_H
#define PLAYER_H

#include "../raylib.h"
#include "../entities.h"

typedef struct Player {
    Entity entity;
    char jump;
    char movement[2];
    float move_speed;
} Player;

void player_init(Player* player);
void player_jump(Player* player);
void player_render(Entity* entity, Vector2 offset);
void player_update(Entity* entity, Tilemap* tm, Vector2 offset, float dt);
#endif // !PLAYER_H
