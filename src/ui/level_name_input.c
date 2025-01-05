#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "../inc/common.h"
#include "../inc/editor.h"
#include "../inc/raylib/raygui.h"

#define MAX_LEVEL_NAME 32

char* levelname_buffer = NULL;

void levelnameinput_init() {
    levelname_buffer = calloc(0, MAX_LEVEL_NAME);
}

void levelnameinput_deinit() {
    free(levelname_buffer);
    levelname_buffer = NULL;
}

void levelnameinput_render() {
    int value = GuiTextInputBox(
        centered_rec(320, 170),
        "Level name",
        "Please enter level name",
        "Submit",
        levelname_buffer,
        MAX_LEVEL_NAME,
        NULL
    );

    if (value == 0) {
        editor_state.popup = NoPopup;
        levelnameinput_deinit();
    }

    if (value == 1) {
        int len = strlen(levelname_buffer);

        // Copy name
        char* name_buffer = malloc(len + 1);
        memcpy(name_buffer, levelname_buffer, len);
        name_buffer[len] = '\0';

        editor_state.level_name = name_buffer;

        levelnameinput_deinit();
        save_level();
        editor_state.popup = NoPopup;
    }
}
