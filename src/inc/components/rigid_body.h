#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <stdbool.h>

#include "../vector.h"
#include "../component.h"
#include "../box2d/box2d.h"
#include "../game_object.h"

typedef struct {
    Component component;

    b2BodyId body_id;
    b2BodyType body_type;

    b2Vec2 velocity;

    float mass;
    float friction;
    float gravity_scale;
    float linear_damping;
    float angular_damping;
    float angular_velocity;

    bool is_sensor;
    bool fixed_rotation;
    bool is_bullet;
} RigidBody;


RigidBody* rigidbody_create(GameObject* go);
void rigidBody_update(Component* component, float dt);
#endif // !RIGID_BODY_H
