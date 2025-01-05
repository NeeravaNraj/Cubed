#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include "game_object.h"

typedef struct {
    void (*init)();
    void (*handle_inputs)();
    void (*update)(float);
    void (*render)();
    void (*deinit)();
} Scene;

const Scene* get_current_scene();
const Vec(GameObject) get_game_objects();

void set_current_scene(Scene* scene);

GameObject* scene_get_game_object(int uid);
void scene_add_game_object(GameObject go);
void scene_remove_game_object(int uid);

void scene_update_gos(float dt);
void scene_render_gos();
#endif // !SCENE_H
