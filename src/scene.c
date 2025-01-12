#include <raylib.h>
#include <stdio.h>
#include <strings.h>
#include <time.h>
#include <stddef.h>
#include "inc/scene.h"
#include "inc/common.h"
#include "inc/game_object.h"
#include "inc/raylib/raylib.h"
#include "inc/raylib/rlgl.h"
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

GameObject* scene_get_game_object_pixel(int x, int y) {
    const RenderTexture2D* rt = &scene->uid_texture;
    Image image = LoadImageFromTexture(rt->texture);
    Color c = GetImageColor(image, x, rt->texture.height - y);
    const char* name = rlGetPixelFormatName(rt->texture.format);
    unsigned int uid = ColorToInt(c);
    UnloadImage(image);

    GameObject* go = scene_get_game_object(uid);

    return go;
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

void scene_render_go_uids() {
    BeginTextureMode(scene->uid_texture);
    rlDisableColorBlend();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    for (int i = 0; i < Vec_length(game_objects); ++i) {
        GameObject* go = &game_objects[i];
        DrawRectangle(
            go->transform.position.x, 
            go->transform.position.y, 
            go->transform.scale.x, 
            go->transform.scale.y, 
            GetColor(go->uid)
        );
    }
    EndMode2D();
    rlEnableColorBlend();
    EndTextureMode();
}
