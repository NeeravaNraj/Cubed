#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "../entities.h"

typedef struct Player {
    Entity entity;
    char movement[2];
} Player;

void init_player(Player* player);
#endif // !PLAYER_H
