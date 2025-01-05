#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

typedef struct {
    Entity entity;

    // TODO: Rendering data
} Player;

void player_init(Player* player, b2WorldId world_id, b2Vec2 position);
void player_handle_input(Player* player);
void player_render(Player* player);
#endif // !PLAYER_H
