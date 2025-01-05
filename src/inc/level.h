#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "tiles.h"
#include "box2d/id.h"
#include "raylib/raylib.h"

#define MAGIC 0xB18B00B2
#define VERSION 0


typedef struct {
    int magic;
    int version;

    size_t tile_length;
} Headers;

typedef struct {
    size_t level_name_len;
    const char* level_name;
} Properties;

typedef struct {
    const char* level_name;
    Vector2 spawn;
    Vector2 end;
} LevelData;

void handle_err(bool cond, char* loc, char* message);

void write_level(const char* name, LevelData* level);
LevelData read_level(const char* path, b2WorldId world_id);

// ---- LEVEL READERS ----

// ----   V0 READER   ----
void header_reader_v0(FILE* file, Headers* headers, LevelData* level);
void tile_reader_v0(FILE* file, Headers* headers, LevelData* level);
void properties_reader_v0(FILE* file, Properties* properties, LevelData* level);
#endif // LEVEL_H
