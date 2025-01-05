#include "inc/common.h"
#include "inc/raylib/raylib.h"

void camera_init() {
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    camera.target.x = 0;
    camera.target.y = 0;

    camera.offset.x = width * 0.5;
    camera.offset.y = height * 0.5;

    camera.rotation = 0;
    camera.zoom = 1;
}
