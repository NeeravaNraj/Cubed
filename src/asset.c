#include "inc/asset.h"
#include "inc/arena.h"
#include "inc/common.h"
#include "inc/tiles.h"
#include "inc/world.h"
#include "inc/raylib.h"
#include <stdint.h>


void load_grass(Assets* assets) {
    Asset asset;
    asset.textures = arena_alloc(sizeof(Texture2D));
    asset.asset_flags = OngridTile;
    asset.kind = GrassPlatform;
    asset.len = 9;

    asset.textures[0] = LoadTexture("assets/tiles/grass.png");
    assets->tile_assets.grass = asset;
}

void load_stone(Assets* assets) {
    Asset asset;
    asset.textures = arena_alloc(sizeof(Texture2D));
    asset.asset_flags = OngridTile;
    asset.kind = StonePlatform;
    asset.len = 9;

    asset.textures[0] = LoadTexture("assets/tiles/stone.png");
    assets->tile_assets.stone = asset;
}

void load_small_decor(Assets* assets) {
    Asset asset;
    asset.textures = arena_alloc(sizeof(Texture2D));
    asset.asset_flags = OffgridTile;
    asset.kind = SmallDecor;
    asset.len = 4;

    asset.textures[0] = LoadTexture("assets/decor/small_decor.png");

    assets->decor_assets.small_decor = asset;
}

void load_menu(Assets* assets) {
    Asset asset;
    asset.textures = arena_alloc(sizeof(Texture2D));
    asset.len = 1;

    asset.textures[0] = LoadTexture("assets/menu.png");

    assets->menu = asset;
}

void load_assets(Assets* assets) {
    load_grass(assets);
    load_stone(assets);
    load_small_decor(assets);
    load_menu(assets);
}

Asset* get_asset(Tiles kind) {
    switch (kind) {
        case StonePlatform: return &assets.tile_assets.stone;
        case GrassPlatform: return &assets.tile_assets.grass;
        default: return NULL;
    }
}
