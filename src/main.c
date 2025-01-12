#include <string.h>
#include "inc/arena.h"
#include "inc/level.h"
#include "inc/asset.h"
#include "inc/scene.h"
#include "inc/window.h"
#include "inc/common.h"
#include "inc/editor.h"
#include "inc/raylib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "inc/raylib/raygui.h"
#include "inc/style_dark.h"

void init() {
    arena_init();
    window_init();
    load_assets();
    set_editor_scene();
}

void loop() {
    float ft = 0;
    float lag = 0;
    float accumulator = 0;

    GuiLoadStyleDark();
    while (!WindowShouldClose()) {
        const Scene* scene = get_current_scene();
        ft = minf(GetFrameTime(), 0.25);
        accumulator += ft;

        // Handle inputs
        scene->handle_inputs();

        // Fixed delay update
        while (accumulator >= FIXED_UPDATE_MS) {
            // TODO: update physics
            scene->update(FIXED_UPDATE_MS);
            accumulator -= FIXED_UPDATE_MS;
        }
        lag = accumulator / FIXED_UPDATE_MS;

        // Rendering
        ClearBackground(BLACK);
        BeginDrawing();
            scene->render();
        EndDrawing();
    }
}

void deinit() {
    get_current_scene()->deinit();
    window_deinit();
    arena_deinit();
}

int main(int ac, char** av) {
    // TODO: make proper arg parser
    init();
    loop();
    deinit();

    return 0;
}
