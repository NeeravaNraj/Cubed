#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../sprites.h"
#include "../component.h"
#include "../game_object.h"

typedef struct {
    Color tint;
    Sprite sprite;
    Component component;
} SpriteRenderer;

SpriteRenderer* spriterenderer_create(GameObject* go, Sprite sprite);
#endif // !RENDERABLE_H
