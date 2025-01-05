#include "inc/game_object.h"
#include "inc/arena.h"
#include "inc/component.h"
#include "inc/hashmap.h"
#include "inc/vector.h"
#include <stddef.h>
#include <string.h>

GameObject* go_create() {
    GameObject* go = arena_alloc(sizeof(GameObject));
    hashmap_init(&go->components);

    return go;
}
void go_update(GameObject* go, float dt) {
    Entry* entry;
    HashMapIterator it;
    hashmap_init_iterator(&it, &go->components);
    while ((entry = hashmap_next_entry(&it))) {
        Component* c = entry->value;
        if (c->update != NULL) {
            c->update(c, dt);
        }
    }
}

void go_render(GameObject* go) {
    Entry* entry;
    HashMapIterator it;
    hashmap_init_iterator(&it, &go->components);
    while ((entry = hashmap_next_entry(&it))) {
        Component* c = entry->value;
        if (c->render!= NULL) {
            c->render(c);
        }
    }
}

Component* go_get_component(GameObject* go, const char* name) {
    const Entry* entry = hashmap_get(&go->components, name);
    return NULL;
}

void go_add_component(GameObject* go, Component* c) {
    hashmap_insert(&go->components, c->name, c);
}

void go_remove_component(GameObject* go, const char* name) {
    hashmap_delete(&go->components, name);
}
