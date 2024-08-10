#ifndef ENTITES_H
#define ENTITES_H

#include <raylib.h>

typedef struct Entity {
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    void* implementor;

    void (*update)(struct Entity*, float);
    void (*render)(struct Entity*);
} Entity;

#endif // !ENTITES_H
