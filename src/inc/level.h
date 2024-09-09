#ifndef LEVEL_H
#define LEVEL_H
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include "./tiles.h"
#include "tilemap.h"
#include "world.h"

#define MAGIC 0xB18B00B2
#define VERSION 0


typedef struct {
    int magic;
    int version;
    Vector2 spawn;
    Vector2 end;
    size_t tile_length;
} Headers;

typedef struct {
    size_t level_name_len;
    char* level_name;
} Properties;

void write_level(char* name, World* world);
void read_level(char* filename, World* world, int version);
#endif // LEVEL_H
