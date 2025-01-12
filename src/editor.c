#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "inc/ui.h"
#include "inc/level.h"
#include "inc/tiles.h"
#include "inc/arena.h"
#include "inc/scene.h"
#include "inc/player.h"
#include "inc/common.h"
#include "inc/camera.h"
#include "inc/editor.h"
#include "inc/sprites.h"
#include "inc/vector.h"
#include "inc/physics.h"
#include "inc/box2d/id.h"
#include "inc/game_object.h"
#include "inc/box2d/types.h"
#include "inc/raylib/rlgl.h"
#include "inc/box2d/box2d.h"
#include "inc/raylib/raylib.h"
#include "inc/raylib/raygui.h"
#include "inc/raylib/raymath.h"
#include "inc/box2d/math_functions.h"

struct EditorState editor_state;
LevelData level;

float CAMERA_SPEED = 16;
float resize_debounce = 0;
Vector2 tile_hovering = {0};

void load_viewport();
void reload_viewport();
void handle_mouse();
void render_hovering_tile();
void handle_camera_movement();
void editor_free_level_name();
bool is_mouse_in_viewport(Vector2 pos);

void editor_init() {
    float height = GetScreenHeight();
    float width = GetScreenWidth();

    editor_state.selected_tile = GrassPlatform;
    editor_state.cursor_mode = Select;
    editor_state.selected_variant = 0;
    editor_state.popup = NoPopup;
    editor_state.level_name = NULL;
    editor_state.selected_go = NULL;
    editor_state.playing = false;

    physics_init();

    load_viewport();
    tileselector_init();
    camera_init();
}

void editor_handle_inputs() {
    handle_mouse();

    if (!editor_state.playing) {
        handle_camera_movement();
    }

    switch (GetKeyPressed()) {
        case KEY_ONE:
            editor_state.cursor_mode = Select;
            break;

        case KEY_TWO:
            editor_state.cursor_mode = Create;
            break;

        case KEY_THREE:
            editor_state.cursor_mode = Delete;
            break;
    }
}

void editor_update(float dt) {
    float height = GetScreenHeight();
    float width = GetScreenWidth();

    float view_width = width - SIDEBAR_WIDTH;
    float view_height = height - TILESELECTOR_HEIGHT;

    camera.offset.x = view_width * 0.5;
    camera.offset.y = view_height * 0.5;

    if (IsWindowResized()) {
        reload_viewport();
    }

    if (resize_debounce > 0) {
        resize_debounce -= FIXED_UPDATE_MS;
    }

    scene_update_gos(dt);

    if (editor_state.playing) {
        physics_update();
    }
}

Scene editor_scene;
void editor_render() {
    scene_render_go_uids();
    BeginTextureMode(editor_state.viewport);
        ClearBackground(BLACK);
        BeginMode2D(camera);
            grid_render();
            scene_render_gos();
            render_hovering_tile();
        EndMode2D();
    EndTextureMode();

    Texture2D viewport = editor_state.viewport.texture;
    DrawTexturePro(
        /* editor_scene.uid_texture.texture,  */
        viewport,
        (Rectangle){
            .x = 0, .y = viewport.height,
            .width = viewport.width,
            .height = -viewport.height
        },
        (Rectangle){
            SIDEBAR_WIDTH, 0,
            viewport.width,
            viewport.height
        },
        vec2(0, 0),
        0, WHITE
    );
    sidebar_render();
    tileselector_render();

    switch (editor_state.popup) {
        case FileViewer:
            fileviewer_render();
            break;

        case LevelNameInput:
            levelnameinput_render();
            break;

        default:
            break;
    }
}

void editor_deinit() {
    editor_free_level_name();
    UnloadRenderTexture(editor_state.viewport);
}

Scene editor_scene = {
    editor_init,
    editor_handle_inputs,
    editor_update,
    editor_render,
    editor_deinit,
};

void set_editor_scene() {
    set_current_scene(&editor_scene);
}

void handle_mouse() {
    Vector2 mouse_position = GetMousePosition();
    bool in_viewport = is_mouse_in_viewport(mouse_position);
    mouse_position.x -= SIDEBAR_WIDTH;
    Vector2 world_mouse_position = GetScreenToWorld2D(mouse_position, camera);
    Vector2 tile_pos = resolve_tile_position(world_mouse_position);
    tile_hovering = to_tile_space(tile_pos);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (in_viewport) {
            if (editor_state.cursor_mode == Create) {
                tile_create(
                    tile_hovering,
                    editor_state.selected_tile,
                    editor_state.selected_variant
                );
            } else if (editor_state.cursor_mode == Select) {
                GameObject* go = scene_get_game_object_pixel(
                    mouse_position.x,
                    mouse_position.y
                );
                if (go != NULL)  {
                    editor_state.selected_go = go;
                }
            } else if (editor_state.cursor_mode == Delete) {
            }
        }
    }

    if (in_viewport && IsKeyDown(KEY_P)) {
        editor_state.playing ^= true;
    }
}

void handle_camera_movement() {
    float width = GetScreenWidth();
    float height = GetScreenHeight();

    if (editor_state.popup == NoPopup) {
        if (IsKeyDown(KEY_A)) {
            camera.target.x -= CAMERA_SPEED;
        }

        if (IsKeyDown(KEY_D)) {
            camera.target.x += CAMERA_SPEED;
        }

        if (IsKeyDown(KEY_W)) {
            camera.target.y -= CAMERA_SPEED;
        }

        if (IsKeyDown(KEY_S)) {
            camera.target.y += CAMERA_SPEED;
        }

        if (IsKeyPressed(KEY_R)) {
            camera.target.x = 0;
            camera.target.y = 0;
            camera.zoom = 1;
        }
    }

    Vector2 mouse_pos = GetMousePosition();
    Vector2 mouse_world_post = GetScreenToWorld2D(mouse_pos, camera);
    
    if (is_mouse_in_viewport(mouse_pos)) {
        camera.zoom += GetMouseWheelMove() * 0.05f;
        camera.zoom = Clamp(camera.zoom, 0.5, 3);
    }
}

void load_viewport() {
    float width = GetScreenWidth() - SIDEBAR_WIDTH;
    float height = GetScreenHeight() - TILESELECTOR_HEIGHT;

    editor_state.viewport = LoadRenderTexture(width, height);
    editor_scene.uid_texture = LoadRenderTexture(width, height);
}

void reload_viewport() {
    if (resize_debounce > 0) return;
    UnloadRenderTexture(editor_state.viewport);
    UnloadRenderTexture(editor_scene.uid_texture);
    load_viewport();
    resize_debounce = FIXED_UPDATE_MS * 4;
}


bool is_mouse_in_viewport(Vector2 pos) {
    return !(
        tileselector_hovered(pos) ||
        sidebar_hovered(pos)
    ) && editor_state.popup == NoPopup;
}

void load_level(const char* path) {
    /* LevelData level = read_level(path, editor_state.world_id); */
    /*  */
    // Load data
    /* editor_free_level_name(); */

    /* editor_state.level_name = level.level_name; */
    /* editor_state.tilemap = level.tilemap; */
}

void save_level() {
    LevelData level;

    assert(editor_state.level_name);
    level.level_name = editor_state.level_name;
    /* level.tilemap = editor_state.tilemap; */

    // TODO
    level.spawn = Vector2Zero();
    level.end = Vector2Zero();

    write_level(level.level_name, &level);
}

void editor_free_level_name() {
    if (editor_state.level_name != NULL) {
        free((char*)editor_state.level_name);
        editor_state.level_name = NULL;
    }
}

void render_hovering_tile() {
    if (editor_state.cursor_mode == Select) {
        DrawRectangleLinesEx(
            (Rectangle){
                .x = tile_hovering.x,
                .y = tile_hovering.y,
                .width = TILE_SIZE, 
                .height = TILE_SIZE,
            }, 
            Clamp(1 / camera.zoom, 1, 2),
            WHITE
        );
    } else if (editor_state.cursor_mode == Create) {
        Sprite sprite = tile_get_sprite(editor_state.selected_tile, editor_state.selected_variant);
        Rectangle src = {
            .x = sprite.x,
            .y = sprite.y,
            .width = sprite.w,
            .height = sprite.h,
        };
        Rectangle dest = {
            .x = tile_hovering.x,
            .y = tile_hovering.y, 
            .width = TILE_SIZE, 
            .height = TILE_SIZE,
        };

        Color tint = WHITE;
        tint.a -= 50;

        DrawTexturePro(
            *sprite.texture,
            src, dest,
            vec2(0, 0), 0,
            tint
        );
    }
}
