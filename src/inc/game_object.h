#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "./hashmap.h"
#include "./transform.h"
#include "./component.h"

typedef struct {
    int uid;
    Transform2D transform;
    HashMap components;
} GameObject;


GameObject* go_create();
void go_update(GameObject*, float);
void go_render(GameObject*);
Component* go_get_component(GameObject*, const char* name);
void go_add_component(GameObject*, Component*);
void go_remove_component(GameObject*, const char* name);
#endif // DEBUG
