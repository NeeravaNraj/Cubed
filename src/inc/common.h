#ifndef COMMON_H
#define COMMON_H

#include "./raylib.h"
#include <unistd.h>
#define WIDTH 1280
#define HEIGHT 720
#define TILE_SIZE 48

typedef  struct {
    Texture2D* textures;
    size_t len;
} Asset;

typedef struct {
    Asset grass;
    Asset stone;
    Asset end;
} Assets;

extern Assets assets;

float minf(float a, float b);
float maxf(float a, float b);
int min(int a, int b);
int max(int a, int b);
float lerpf(float t, float a, float b);

#endif // !COMMON_H
