#ifndef COMMON_H
#define COMMON_H

#include "./raylib.h"
#include <unistd.h>
#include "asset.h"

#define WIDTH 1280
#define HEIGHT 720
#define TILE_SIZE 48
#define TEXTURE_SIZE 32
#define FIXED_UPDATE_MS 0.02

extern Assets assets;
extern float FRAME_RATE;

// Math functions
float minf(float a, float b);
float maxf(float a, float b);
int min(int a, int b);
int max(int a, int b);

float distf(float a, float b);
float lerpf(float t, float a, float b);
bool aroundf(float precision, float a, float b);

// Math easing functions
float sin_ease_out(float t);
float quadratic_ease_outf(float t);

Vector2 vec2(float x, float y);

Rectangle rect_from_tile(Tile* tile);
Vector2 to_tile_space(Vector2 position);

int int_in_array(int* array, size_t length, int value);
int float_in_array(float* array, size_t length, float value);
#endif // !COMMON_H
