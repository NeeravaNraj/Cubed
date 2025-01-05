#ifndef ENTITY_H
#define ENTITY_H

#include "./box2d/box2d.h"

typedef struct {
    b2BodyId body_id;
    b2Vec2 extent;
} Entity;


/* Entity* entity_init(b2WorldId worldId, b2Vec2 position, b2Vec2 extent); */
/* void entity_render(Entity* entity); */
/* void entity_destructor(void* entity); */
/* void entity_deinit(Entity* entity); */
#endif // !ENTITY_H
