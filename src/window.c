#include "inc/window.h"
#include "inc/raylib/raylib.h"

const int TARGET_FPS = 60;
const float INITIAL_WIDTH = 1280;
const float INITIAL_HEIGHT = 720;

void window_init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(TARGET_FPS);

    InitWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "Cubed");
    SetWindowMinSize(INITIAL_WIDTH, INITIAL_HEIGHT);
}

void window_deinit() {
    CloseWindow();
}
