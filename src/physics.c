#include <string.h>
#include "inc/box2d/math_functions.h"
#include "inc/box2d/types.h"
#include "inc/common.h"
#include "inc/physics.h"
#include "inc/box2d/id.h"
#include "inc/box2d/box2d.h"
#include "inc/components/rigid_body.h"
#include "inc/hashmap.h"

#define GRAVITY 10
#define PIXELS_PER_METRE (TILE_SIZE)

Physics physics_create() {
    Physics physics;
    b2SetLengthUnitsPerMeter(PIXELS_PER_METRE);

    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity.y = GRAVITY * PIXELS_PER_METRE;

    physics.world_id = b2CreateWorld(&world_def);
    physics.timestep = FIXED_UPDATE_MS;
    physics.sub_steps = 4;

    return physics;
}

void physics_update(Physics* physics) {
    b2World_Step(physics->world_id, physics->timestep, physics->sub_steps);
}

void physics_deinit(Physics* physics) {
    b2DestroyWorld(physics->world_id);
    physics->world_id = b2_nullWorldId;
}

void physics_add(Physics *self, GameObject *go) {
    const Entry* entry = hashmap_get(&go->components, "Cubed.RigidBody");
    if (!entry) return;
    Component* component = entry->value;
    RigidBody* rb = component->implementor;
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

    rb->body_id = b2CreateBody(self->world_id, &body_def);

    
}
