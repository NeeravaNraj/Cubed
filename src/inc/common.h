#ifndef COMMON_H
#define COMMON_H

#include "./raylib.h"
#include <unistd.h>
#include "asset.h"

#define WIDTH 1280
#define HEIGHT 720
#define TILE_SIZE 48
#define TEXTURE_SIZE 32

extern Assets assets;
extern float FRAME_RATE;

float minf(float a, float b);
float maxf(float a, float b);
int min(int a, int b);
int max(int a, int b);
float lerpf(float t, float a, float b);
Vector2 vec2(float x, float y);
#endif // !COMMON_H
