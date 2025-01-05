#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "../component.h"
#include "../box2d/box2d.h"
#include "../game_object.h"

typedef struct {
    Component component;

    b2Vec2 origin;
    b2Vec2 offset;
    b2Vec2 half_size;
} BoxCollider;


BoxCollider* boxcollider_create(GameObject* go);
#endif // !BOX_COLLIDER_H
