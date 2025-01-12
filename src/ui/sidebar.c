#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "../inc/ui.h"
#include "../inc/editor.h"
#include "../inc/common.h"
#include "../inc/raylib/raylib.h"
#include "../inc/raylib/raygui.h"

Rectangle sidebar_bounds = {0};
Vector2 sidebar_content_size = {0};
Vector2 sidebar_scroll = {0};

Vector2 sidebar_icon_button_size = {
    24, 24
};

int padding = 10;


void on_file_click(const char* file);
bool small_button_render(Vector2 position, const char* text);

void sidebar_drawcontent(Vector2 position, Vector2 scroll) {
    Vector2 open_fileviewer_button_pos = vec2(position.x + padding, position.y + padding);
    if (small_button_render(open_fileviewer_button_pos, GuiIconText(ICON_FILE, " Open"))) {
        fileviewer_init(LEVELS_DIR, on_file_click);
        editor_state.popup = FileViewer;
    }

    Vector2 save_level_button_pos = open_fileviewer_button_pos;
    save_level_button_pos.y += padding + 24;
    if (small_button_render(save_level_button_pos, GuiIconText(ICON_FILE_SAVE_CLASSIC, " Save"))) {
        if (editor_state.level_name == NULL) {
            levelnameinput_init();
            editor_state.popup = LevelNameInput;
        } else {
            save_level();
        }
    }

    if (editor_state.selected_go != NULL) {
        GameObject* go = editor_state.selected_go;

        Font font = GetFontDefault();
        const char* gameobject_label_text = "Selected GameObject:";
        Rectangle gameobject_label;
        gameobject_label.x = save_level_button_pos.x;
        gameobject_label.y = save_level_button_pos.y + 24 + padding * 2;
        gameobject_label.width = strlen(gameobject_label_text) * 8;
        gameobject_label.height = 24;
        GuiLabel(gameobject_label, gameobject_label_text);

        /* Rectangle spinner_bounds = gameobject_label; */
        /* spinner_bounds.y += spinner_bounds.height + padding; */
        /* spinner_bounds.height = 32; */
        /* spinner_bounds.width = 100; */
        /*  */
        /* GuiSpinner(spinner_bounds, "X", &go->transform.position.x, INT_MIN, INT_MAX, true); */
    }
}

void sidebar_render() {
    float height = GetScreenHeight();
    float width = GetScreenWidth();
    
    sidebar_bounds.x = 0;
    sidebar_bounds.y = 0;
    sidebar_bounds.width = SIDEBAR_WIDTH;
    sidebar_bounds.height = height;

    sidebar_content_size.x = 200;
    sidebar_content_size.y = 100;
    GuiWindow(sidebar_bounds, sidebar_content_size, &sidebar_scroll, sidebar_drawcontent);
}

bool sidebar_hovered(Vector2 pos) {
    return CheckCollisionPointRec(pos, sidebar_bounds);
}

bool small_button_render(Vector2 position, const char* text) {
    Font font = GetFontDefault();
    int text_width = MeasureText(text, font.baseSize);

    Rectangle bounds = {
        .x = position.x,
        .y = position.y,
        .width = text_width + 8,
        .height = 24,
    };

    return GuiButton(bounds, text);
}

void on_file_click(const char* file) {
    int file_len = strlen(file);
    int total_len = sizeof(LEVELS_DIR"/") + file_len;

    char* path = malloc(total_len + 1);
    sprintf(path, "%s/%s",LEVELS_DIR, file); 
    path[total_len] = '\0';
    load_level(path);
    /* free(path); */
    editor_state.popup = NoPopup;
}
