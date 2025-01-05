#include "../inc/arena.h"
#include "../inc/common.h"
#include "../inc/components/sprite_renderer.h"


void spriterenderer_render(Component* c);

SpriteRenderer* spriterenderer_create(GameObject* go, Sprite sprite) {
    SpriteRenderer* r = arena_alloc(sizeof(SpriteRenderer));

    r->component.name = "Cubed.SpriteRenderer";

    r->sprite = sprite;
    r->tint = WHITE;
    r->component.parent = go;
    r->component.implementor = r;

    r->component.init = NULL;
    r->component.update = NULL;
    r->component.deinit = NULL;
    r->component.render = spriterenderer_render;

    return r;
}

void spriterenderer_render(Component* c) {
    SpriteRenderer* self = c->implementor;
    Transform2D transform = ((GameObject*)c->parent)->transform;

    Rectangle src = {
        .x = self->sprite.x,
        .y = self->sprite.y,
        .width = self->sprite.w,
        .height = self->sprite.h,
    };
    Rectangle dest = {
        .x = transform.position.x,
        .y = transform.position.y,
        .height = transform.scale.x,
        .width = transform.scale.y,
    };

    DrawTexturePro(
        *self->sprite.texture,
        src, dest, vec2(0, 0),
        transform.rotation,
        self->tint
    );
}
