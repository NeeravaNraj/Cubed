#ifndef EDITOR_H
#define EDITOR_H

#include "box2d/id.h"
#include "entity.h"
#include "game_object.h"
#include "physics.h"
#include "player.h"
#include "tiles.h"
#include "vector.h"

typedef enum {
    NoPopup = 0,
    FileViewer,
    LevelNameInput,
    PopupsEnd
} Popups;

typedef enum {
    Select = 1,
    Create,
    Delete,
} CursorMode;

struct EditorState {
    const char* level_name;

    GameObject* selected_go;
    char selected_variant;
    Tiles selected_tile;


    CursorMode cursor_mode;
    Popups popup;
    bool playing;

    RenderTexture2D viewport;
    /* TileType selected_tile_type; */
};
extern struct EditorState editor_state;

void set_editor_scene();
void save_level();
void load_level(const char* path);
#endif // !EDITOR_H
