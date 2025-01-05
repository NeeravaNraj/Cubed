#ifndef PHYSICS_H
#define PHYSICS_H

#include "box2d/box2d.h"
#include "game_object.h"

typedef struct {
    b2WorldId world_id;

    int sub_steps;

    float gravity;
    float timestep;
} Physics;

void physics_init();
void physics_update();
void physics_deinit();

void physics_add(GameObject* go);
void physics_remove(GameObject* go);
#endif // !PHYSICS_H
