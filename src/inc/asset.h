#ifndef ASSET_H
#define ASSET_H

#include "tiles.h"
#include <unistd.h>
#include "./raylib/raylib.h"

typedef struct {
    Texture2D* textures;
    size_t len;
    unsigned char asset_flags;
    unsigned char kind;
} Asset;

/* --- Tile Assets --- */
typedef struct {
    Asset grass;
    Asset stone;
} TileAssets;

/* --- Decor Assets --- */
typedef struct {
    Asset small_decor;
} DecorAssets;

typedef struct {
    TileAssets tile_assets;
    DecorAssets decor_assets;
    Asset menu;
} Assets;

#endif // !ASSET_H
