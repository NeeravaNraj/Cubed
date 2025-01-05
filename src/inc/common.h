#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include "./raylib.h"
#include "raylib/raylib.h"
#include "box2d/math_functions.h"

#define TILE_SIZE 32
#define TEXTURE_SIZE 32
#define DEATH_HEIGHT 20

#define FILE_EXT ".cbd"
#define LEVELS_DIR "levels"

extern float FRAME_RATE;
extern const float FIXED_UPDATE_MS;
extern Camera2D camera;

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
Rectangle centered_rec(float width, float height);

Vector2 to_tile_space(Vector2 position);

float get_rect_overlap_vert(Rectangle a, Rectangle b);
float get_rect_overlap_hor(Rectangle a, Rectangle b);
int int_in_array(int* array, size_t length, int value);
int float_in_array(float* array, size_t length, float value);

void print_vec2(Vector2 v);

int signum(int value);
float signumf(float value);

b2Vec2 asb2vec2(Vector2 vec);
Vector2 asvec2(b2Vec2 vec);

int get_id(int counter);
#endif // !COMMON_H
