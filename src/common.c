#include "inc/common.h"
#include "inc/raylib.h"

float minf(float a, float b) {
    return a < b ? a : b;
}

float maxf(float a, float b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

float lerpf(float t, float a, float b) {
    return a + (b - a) * t;
}

Vector2 vec2(float x, float y) {
    return (Vector2){ .x = x, .y = y};
}
