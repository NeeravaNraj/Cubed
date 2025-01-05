#include <stdio.h>
#include <string.h>
#include "inc/common.h"
#include "inc/hashmap.h"
#include "inc/physics.h"
#include "inc/box2d/id.h"
#include "inc/component.h"
#include "inc/box2d/box2d.h"
#include "inc/box2d/types.h"
#include "inc/box2d/math_functions.h"
#include "inc/components/rigid_body.h"
#include "inc/components/box_collider.h"

#define GRAVITY 10
#define PIXELS_PER_METRE (TILE_SIZE)

Physics physics;

void physics_add_boxcollider(RigidBody* rigid_body, BoxCollider* collider);

void physics_init() {
    b2SetLengthUnitsPerMeter(PIXELS_PER_METRE);

    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity.y = GRAVITY * PIXELS_PER_METRE;

    physics.world_id = b2CreateWorld(&world_def);
    physics.timestep = FIXED_UPDATE_MS;
    physics.sub_steps = 4;
}

void physics_update() {
    b2World_Step(physics.world_id, physics.timestep, physics.sub_steps);
}

void physics_deinit() {
    b2DestroyWorld(physics.world_id);
    physics.world_id = b2_nullWorldId;
}

void physics_add(GameObject *go) {
    const Entry* entry = hashmap_get(&go->components, component_names[CubedRigidBody]);
    if (!entry) return;
    Component* component = entry->value;
    RigidBody* rb = component->implementor;
    
    if (!B2_ID_EQUALS(rb->body_id, b2_nullBodyId)) {
        fprintf(stderr, "ERROR: '%s' already has a body in world, cannot re-add body.\n", component_names[CubedRigidBody]);
        assert(false);
    }

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = rb->body_type;
    body_def.position = asb2vec2(go->transform.position);
    body_def.rotation = b2MakeRot(go->transform.rotation);

    body_def.linearVelocity = rb->velocity;
    body_def.gravityScale = rb->gravity_scale;
    body_def.linearDamping = rb->linear_damping;
    body_def.angularDamping = rb->angular_damping;
    body_def.angularVelocity = rb->angular_velocity;

    body_def.isBullet = rb->is_bullet;
    body_def.fixedRotation = rb->fixed_rotation;

    body_def.userData = go;

    rb->body_id = b2CreateBody(physics.world_id, &body_def);


    if ((entry = hashmap_get(&go->components, component_names[CubedBoxCollider])) != NULL) {
        physics_add_boxcollider(rb, entry->value);
    } else {
        fprintf(stderr, "ERROR: Could not find collider for '%s'\n", component_names[CubedRigidBody]);
        assert(false);
    }
}

void physics_add_boxcollider(RigidBody* rigid_body, BoxCollider* collider) {
    if (B2_ID_EQUALS(rigid_body->body_id, b2_nullBodyId)) {
        fprintf(stderr, "ERROR: Expected valid body - got 'null'!\n");
        assert(false);
    }

    b2Rot rotation = b2Body_GetRotation(rigid_body->body_id);
    b2Vec2 position = b2Body_GetPosition(rigid_body->body_id);
    position.x += collider->offset.x;
    position.y += collider->offset.y;
    b2Body_SetTransform(rigid_body->body_id, position, rotation);

    b2Polygon box = b2MakeBox(collider->half_size.x, collider->half_size.y);
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.userData = rigid_body->component.parent;
    shape_def.friction = rigid_body->friction;
    shape_def.isSensor = rigid_body->is_sensor;
    // TODO: configurable
    shape_def.density = 1; 
    b2CreatePolygonShape(rigid_body->body_id, &shape_def, &box);
}
