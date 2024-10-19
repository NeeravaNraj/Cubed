#include <math.h>
#include <unistd.h>
#include "inc/common.h"
#include "inc/raylib/raylib.h"

inline float minf(float a, float b) {
    return a < b ? a : b;
}

inline float maxf(float a, float b) {
    return a > b ? a : b;
}

inline int min(int a, int b) {
    return a < b ? a : b;
}

inline int max(int a, int b) {
    return a > b ? a : b;
}


inline float lerpf(float t, float a, float b) {
    return (1 - t) * a + t * b;
}

inline float quadratic_ease_outf(float t) {
    return 1 - powf(1 - t, 2);
}

inline float sin_ease_out(float t) {
    return sinf(t * PI / 2);
}

inline Vector2 vec2(float x, float y) {
    return (Vector2){ .x = x, .y = y};
}


inline bool aroundf(float precision, float a, float b) {
    return fabsf(a - b) <= precision;
}

inline float distf(float a, float b) {
    return fabs(b - a);
}

inline Rectangle rect_from_tile(Tile* tile) {
    return (Rectangle){
        .x = tile->position.x,
        .y = tile->position.y,
        .width = TILE_SIZE,
        .height = TILE_SIZE,
    };
}

Vector2 to_tile_space(Vector2 position) {
    int tile_x = position.x / TILE_SIZE;
    int tile_y = position.y / TILE_SIZE;

    position.x = tile_x * TILE_SIZE;
    position.y = tile_y * TILE_SIZE;

    return position;
}


int int_in_array(int* array, size_t length, int value) {
    for (int i = 0; i < length; ++i) {
        if (value == array[i]) return i;
    }
    return -1;
}


int float_in_array(float* array, size_t length, float value) {
    for (int i = 0; i < length; ++i) {
        if (value == array[i]) return i;
    }
    return -1;
}
