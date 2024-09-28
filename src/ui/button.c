#include "../inc/ui/button.h"
#include "../inc/raylib.h"

bool button_hovered(Button* button, Vector2 pointer_position) {
    Rectangle button_rect = {
        .x = button->position.x,
        .y = button->position.y,
        .width = button->size.x,
        .height = button->size.y,
    };

    return CheckCollisionPointRec(pointer_position, button_rect);
}

bool button_clicked(Button* button, Vector2 pointer_position) {
    return button_hovered(button, pointer_position) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void render_button(Button* button) {
    if (button->style.border_size) {
        Vector2 border_pos = {
            .x = button->position.x - (float)button->style.border_size / 2,
            .y = button->position.y - (float)button->style.border_size / 2,
        };
        Vector2 border_size = {
            .x = button->size.x + button->style.border_size,
            .y = button->size.y + button->style.border_size,
        };
        DrawRectangleV(border_pos, border_size, button->style.border_color);
    }

    DrawRectangleV(button->position, button->size, button->style.background_color);
    float text_width = MeasureText(button->label, button->style.font_size);
    DrawText(
        button->label,
        button->position.x + button->size.x / 2 - text_width / 2,
        button->position.y + button->size.y / 2 - button->style.font_size / 2,
        button->style.font_size,
        button->style.font_color
    );
}
