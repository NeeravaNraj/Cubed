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

Physics physics_create();
void physics_update(Physics* physics);
void physics_add(Physics* physics, GameObject* go);
void physics_remove(Physics* physics, GameObject* go);
void physics_deinit(Physics* physics);
#endif // !PHYSICS_H
