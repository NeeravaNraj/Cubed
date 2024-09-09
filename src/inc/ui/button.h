#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>

typedef struct {
    char* label;
    float font_size;
    Color font_color;

    Vector2 position;
    Vector2 size;

    Color background_color;

    int border_size;
    Color border_color;

    void (*onclick)();
} Button;

void render_button(Button* button);
#endif // !BUTTON_H
