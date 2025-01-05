#ifndef ASSET_H
#define ASSET_H

#include <stdbool.h>

#include "hashmap.h"
#include "./sprites.h"
#include "./raylib/raylib.h"

extern HashMap* assets;

void load_assets();
SpriteSheet* get_asset(const char* name);
#endif // !ASSET_H
