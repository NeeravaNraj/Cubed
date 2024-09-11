#include "inc/arena.h"
#include "inc/common.h"
#include "inc/world.h"
#include "inc/raylib.h"
#include <stdint.h>
#include <stdio.h>

void load_grass(Assets* assets) {
    Asset asset;
    asset.textures = arena_alloc(sizeof(Texture2D));
    asset.len = 1;

    char buffer[32];
    for (uint8_t i = 0; i < asset.len; ++i) {
        sprintf(buffer, "assets/tiles/grass/%d.png", i); 
        asset.textures[i] = LoadTexture(buffer);
    }

    assets->grass = asset;
}

void load_stone(Assets* assets) {
    Asset asset;
    asset.textures = arena_alloc(sizeof(Texture2D));
    asset.len = 2;

    char buffer[32];
    for (uint8_t i = 0; i < asset.len; ++i) {
        sprintf(buffer, "assets/tiles/stone/%d.png", i); 
        asset.textures[i] = LoadTexture(buffer);
    }

    assets->stone = asset;
}

void load_end(Assets* assets) {
    Asset asset;
    asset.textures = arena_alloc(sizeof(Texture2D));
    asset.len = 1;

    char buffer[32];
    for (uint8_t i = 0; i < asset.len; ++i) {
        sprintf(buffer, "assets/tiles/end/%d.png", i); 
        asset.textures[i] = LoadTexture(buffer);
    }

    assets->end = asset;
}
void load_assets(Assets* assets) {
    load_grass(assets);
    load_stone(assets);
    load_end(assets);
}
