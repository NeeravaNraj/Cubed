#include "../inc/arena.h"
#include "../inc/components/rigid_body.h"

RigidBody* rigidbody_create(GameObject* go) {
    RigidBody* rb = arena_alloc(sizeof(RigidBody));

    rb->component.name = component_names[CubedRigidBody];

    rb->component.parent = go;
    rb->component.implementor = rb;
    
    rb->body_id = b2_nullBodyId;
    rb->body_type = b2_staticBody;

    rb->velocity.x = 0;
    rb->velocity.y = 0;

    rb->mass = 0;
    rb->friction = 0;
    rb->gravity_scale = 1;
    rb->linear_damping = 0;
    rb->angular_damping = 0;
    rb->angular_velocity = 0;

    rb->is_sensor = false;
    rb->fixed_rotation = false;
    rb->is_bullet= false;
    
    rb->component.init = NULL;
    rb->component.render = NULL;
    rb->component.deinit = NULL;

    rb->component.update = rigidBody_update;

    return rb;
}

void rigidBody_update(Component* component, float dt) {
    RigidBody* self = component->implementor;
    GameObject* go = component->parent;

    if (!B2_ID_EQUALS(self->body_id, b2_nullBodyId)) {
        b2Vec2 position = b2Body_GetPosition(self->body_id);
        b2Rot rotation = b2Body_GetRotation(self->body_id);

        go->transform.position.x = position.x;
        go->transform.position.y = position.y;
        go->transform.rotation =  b2Rot_GetAngle(rotation);
    }
}
