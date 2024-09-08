#ifndef LEVEL_H
#define LEVEL_H
#include <raylib.h>
#include <stddef.h>
#include "./tiles.h"
#include "tilemap.h"

#define MAGIC 0xB18B00B2
#define VERSION 0


typedef struct {
    int magic;
    int version;
    size_t tile_length;
} Headers;

typedef struct {
    int level_name_len;
    char* level_name;
} Properties;

void write_level(char* name, Tilemap* map);
void read_level(char* filename, Tilemap* map);
#endif // LEVEL_H
