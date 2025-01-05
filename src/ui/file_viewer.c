#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../inc/ui.h"
#include "../inc/editor.h"
#include "../inc/common.h"
#include "../inc/system.h"
#include "../inc/vector.h"
#include "../inc/raylib/raylib.h"
#include "../inc/raylib/raygui.h"

Rectangle fileviewer_bounds = {0};
Vector2 fileviewer_scroll = {0};
Vector2 fileviewer_content_size = {140, 320};

const char* PATH = NULL;
Vec(const char*) files = NULL;

void file_reader(const char* file);
void fileentry_desctructor(void* ptr);
void (*file_onclick)(const char* file) = NULL;


void fileviewer_init(const char* path, void (*onclick)(const char*)) {
    PATH = path;
    file_onclick = onclick;
    Vec_init(files, 8, fileentry_desctructor);

    // Load dir content
    list_files_in_directory(PATH, file_reader);
}

void fileviewer_drawcontent(Vector2 position, Vector2 scroll) {
    int padding = 10;
    int entry_height = 24;
    int top_row_height = 20;

    if (Vec_length(files) > 0) {
        for (int i = 0; i < Vec_length(files); ++i) {
            Vector2 row_position = {0};
            row_position.x = position.x + padding + scroll.x;
            row_position.y = position.y + i * 24 + padding + scroll.y;
            row_position.y += top_row_height;

            Rectangle bounds = {
                .x = row_position.x,
                .y = row_position.y,
                .width = fileviewer_bounds.width - padding * 2,
                .height = 24,
            };
            const char* text = GuiIconText(ICON_FILE, GetFileNameWithoutExt(files[i]));
            if (GuiLabelButton(bounds, text)) {
                file_onclick(files[i]);
            }
        }
    } else {
        Font default_font = GetFontDefault();
        const char* text = "No levels found!";
        int text_width = MeasureText(text, default_font.baseSize);

        GuiLabel((Rectangle){
            .x = position.x + fileviewer_bounds.width / 2 - (float)(text_width + 12) / 2,
            .y = position.y + top_row_height + padding * 2,
            .width = text_width * 2,
            .height = 24,
        }, text);
    }

    fileviewer_content_size.x = 380;
    fileviewer_content_size.y = (Vec_length(files) * entry_height + padding);
}

void fileviewer_render() {
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    fileviewer_bounds.width = 400;
    fileviewer_bounds.height = 200;
    fileviewer_bounds.x = width * 0.5 - fileviewer_bounds.width * 0.5;
    fileviewer_bounds.y = height * 0.5 - fileviewer_bounds.height* 0.5;

    int padding = 10;
    GuiWindow(fileviewer_bounds, fileviewer_content_size, &fileviewer_scroll, fileviewer_drawcontent);

    Rectangle cover = {
        .x = fileviewer_bounds.x + 1,
        .y = fileviewer_bounds.y + 1,
        .width = fileviewer_bounds.width - 2,
        .height = padding + 23,
    };

    DrawRectangleRec(cover, GetColor(0x3c3c3cff));

    const char* title = "Levels";
    Font default_font = GetFontDefault();
    int text_width = MeasureText(title, default_font.baseSize);
    Rectangle title_bounds = {
        .x = fileviewer_bounds.x + padding,
        .y = fileviewer_bounds.y + padding * 0.5,
        .width = text_width,
        .height = 24
    };

    DrawText(title, title_bounds.x, title_bounds.y, 20, GetColor(0xc3c3c3ff));

    int button_size = 24;
    const char* close_icon = GuiIconText(ICON_CROSS, "");
    Rectangle close_button_bounds = {
        .x = fileviewer_bounds.x + fileviewer_bounds.width - button_size - padding * 0.5,
        .y = fileviewer_bounds.y + padding * 0.5,
        .width = button_size,
        .height = button_size,
    };


    if (GuiButton(close_button_bounds, close_icon)) {
        fileviewer_deinit();
    }
}

void fileviewer_deinit() {
    Vec_free(files);
    PATH = NULL;
    files = NULL;
    file_onclick = NULL;
    editor_state.popup = NoPopup;
}


void file_reader(const char* entry) {
    if (IsFileExtension(entry, FILE_EXT)) {
        int entry_len = strlen(entry);
        char* file = malloc(entry_len + 1);
        memcpy(file, entry, entry_len);

        file[entry_len] = '\0';

        Vec_push(files, file);
    }
}

void fileentry_desctructor(void* ptr) {
    free(*((void**)ptr));
}
