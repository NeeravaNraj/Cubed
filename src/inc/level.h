#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "tiles.h"
#include "world.h"
#include "raylib.h"
#include "tilemap.h"

#define MAGIC 0xB18B00B2
#define VERSION 1


typedef struct {
    int magic;
    int version;
    Vector2 spawn;
    Vector2 end;
    size_t tile_length;
    size_t offgrid_tile_length;
} Headers;

typedef struct {
    size_t level_name_len;
    char* level_name;
} Properties;

void handle_err(bool cond, char* loc, char* message);

void write_level(char* name, World* world);
void read_level(char* filename, World* world);

// ---- LEVEL READERS ----

// ----   V0 READER   ----
void header_reader_v0(FILE* file, Headers* headers);
void tile_reader_v0(FILE* file, Headers* headers, World* world);
void properties_reader_v0(FILE* file, Properties* properties);

// ----   V1 READER   ----
void header_reader_v1(FILE* file, Headers* headers);
void tile_reader_v1(FILE* file, Headers* headers, World* world);
void properties_reader_v1(FILE* file, Properties* properties);
#endif // LEVEL_H
