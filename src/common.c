#include <math.h>
#include <stdio.h>
#include <raylib.h>
#include "inc/common.h"
#include "inc/raylib/raylib.h"

const float FIXED_UPDATE_MS = 1.0f / 60.0f;

inline float minf(float a, float b) { return a < b ? a : b; }

inline float maxf(float a, float b) { return a > b ? a : b; }

inline int min(int a, int b) { return a < b ? a : b; }

inline int max(int a, int b) { return a > b ? a : b; }

inline float lerpf(float t, float a, float b) { return (1 - t) * a + t * b; }

inline float quadratic_ease_outf(float t) { return 1 - powf(1 - t, 2); }

inline float sin_ease_out(float t) { return sinf(t * PI / 2); }

inline Vector2 vec2(float x, float y) { return (Vector2){.x = x, .y = y}; }

inline bool aroundf(float precision, float a, float b) {
  return fabsf(a - b) <= precision;
}

inline float distf(float a, float b) { return fabs(b - a); }

Vector2 to_tile_space(Vector2 position) {
  int tile_x = position.x / TILE_SIZE;
  int tile_y = position.y / TILE_SIZE;

  position.x = tile_x * TILE_SIZE;
  position.y = tile_y * TILE_SIZE;

  return position;
}

int int_in_array(int *array, size_t length, int value) {
  for (int i = 0; i < length; ++i) {
    if (value == array[i])
      return i;
  }
  return -1;
}

int float_in_array(float *array, size_t length, float value) {
  for (int i = 0; i < length; ++i) {
    if (value == array[i])
      return i;
  }
  return -1;
}

float get_rect_overlap_vert(Rectangle a, Rectangle b) {
  return minf(a.y + a.height, b.y + b.height) - maxf(a.y, b.y);
}

float get_rect_overlap_hor(Rectangle a, Rectangle b) {
  return minf(a.x + a.width, b.x + b.width) - maxf(a.x, b.x);
}

b2Vec2 to_b2vec2(Vector2 vec) {
    return (b2Vec2) { vec.x, vec.y };
}

void print_vec2(Vector2 v) {
    printf("(%f, %f)\n", v.x, v.y);
}

int signum(int value) {
    return (0 < value) - (value < 0);
}

float signumf(float value) {
    return (0 < value) - (value < 0);
}

Rectangle centered_rec(float width, float height) {
    float screen_w = GetScreenWidth();
    float screen_h = GetScreenHeight();

    return (Rectangle){
        .x = screen_w * 0.5 - width * 0.5,
        .y = screen_h * 0.5 - height * 0.5,
        .width = width,
        .height = height,
    };
}

b2Vec2 asb2vec2(Vector2 vec) {
    return (b2Vec2){ .x = vec.x, .y = vec.y };
}

Vector2 asvec2(b2Vec2 vec) {
    return (Vector2){ .x = vec.x, .y = vec.y };
}
