#ifndef ENTITES_H
#define ENTITES_H

#include "raylib.h"
#include "tilemap.h"

typedef struct Entity {
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    void* implementor;

    void (*update)(struct Entity*, Tilemap*, Vector2, float);
    void (*render)(struct Entity*, Vector2);
} Entity;

#endif // !ENTITES_H
