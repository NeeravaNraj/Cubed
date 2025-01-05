#include <time.h>
#include <stddef.h>
#include "inc/scene.h"
#include "inc/common.h"
#include "inc/game_object.h"
#include "inc/vector.h"

const Scene* scene = NULL;
Vec(GameObject) game_objects = NULL;

const Scene* get_current_scene() {
    return scene;
}

const Vec(GameObject) get_game_objects() {
    return game_objects;
}

void set_current_scene(Scene* s) {
    if (game_objects == NULL) {
        Vec_init(game_objects, 8, NULL);
    }

    if (scene != NULL) {
        scene->deinit();
    }

    scene = s;
    s->init();
}

GameObject* scene_get_game_object(int uid) {
    for (int i = 0; i < Vec_length(game_objects); ++i) {
        if (game_objects[i].uid == uid) return &game_objects[i];
    }

    return NULL;
}

void scene_add_game_object(GameObject go) {
    go.uid = get_id(Vec_length(game_objects));
    Vec_push(game_objects, go);
}

void scene_remove_game_object(int uid) {
    for (int i = 0; i < Vec_length(game_objects); ++i) {
        if (game_objects[i].uid == uid) {
            Vec_delete(game_objects, i);
        }
    }
}


void scene_update_gos(float dt) {
    for (int i = 0; i < Vec_length(game_objects); ++i) {
        GameObject* go = &game_objects[i];
        go_update(go, dt);
    }
}

void scene_render_gos() {
    for (int i = 0; i < Vec_length(game_objects); ++i) {
        GameObject* go = &game_objects[i];
        go_render(go);
    }
}
