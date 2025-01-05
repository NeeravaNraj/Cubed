#include "../inc/arena.h"
#include "../inc/raylib/raylib.h"
#include "../inc/components/box_collider.h"

void boxcollider_render(Component* component);

BoxCollider* boxcollider_create(GameObject* go) {
    BoxCollider* box_collider = arena_alloc(sizeof(BoxCollider));

    box_collider->component.name = component_names[CubedBoxCollider];

    box_collider->component.parent = go;
    box_collider->component.implementor = box_collider;

    box_collider->offset.x = 0;
    box_collider->offset.y = 0;
    box_collider->origin.x = 0;
    box_collider->origin.y = 0;
    box_collider->half_size.x = 1;
    box_collider->half_size.y = 1;

    box_collider->component.init = NULL;
    box_collider->component.update = NULL;
    box_collider->component.deinit = NULL;

    box_collider->component.render = boxcollider_render;

    return box_collider;
}

void boxcollider_render(Component* component) {
    BoxCollider* self = component->implementor;
    GameObject* go = component->parent;

    Rectangle rect = {
        .x = go->transform.position.x,
        .y = go->transform.position.y,
        .width = self->half_size.x * 2,
        .height = self->half_size.y * 2,
    };

    DrawRectangleLinesEx(rect, 1, GREEN);
}
