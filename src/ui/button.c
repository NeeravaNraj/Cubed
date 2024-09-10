#include "../inc/ui/button.h"
#include "../inc/raylib.h"


void render_button(Button* button) {
    if (button->border_size) {
        Vector2 border_pos = {
            .x = button->position.x - (float)button->border_size / 2,
            .y = button->position.y - (float)button->border_size / 2,
        };
        Vector2 border_size = {
            .x = button->size.x + button->border_size,
            .y = button->size.y + button->border_size,
        };
        DrawRectangleV(border_pos, border_size, button->border_color);
    }

    DrawRectangleV(button->position, button->size, button->background_color);
    float text_width = MeasureText(button->label, button->font_size);
    DrawText(
        button->label,
        button->position.x + button->size.x / 2 - text_width / 2,
        button->position.y + button->size.y / 2 - button->font_size / 2,
        button->font_size,
        button->font_color
    );
}
