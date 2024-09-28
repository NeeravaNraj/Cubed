#ifndef BUTTON_H
#define BUTTON_H

#include "../raylib.h"
#include "style.h"

typedef struct {
    char* label;
    Vector2 position;
    Vector2 size;
    Style style;
} Button;

bool button_hovered(Button* button, Vector2 pointer_position);
bool button_clicked(Button* button, Vector2 pointer_position);
void render_button(Button* button);
#endif // !BUTTON_H
