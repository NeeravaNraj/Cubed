#ifndef STYLE_H
#define STYLE_H

#include "../raylib.h"

typedef struct {
    float font_size;
    Color font_color;

    Color background_color;
    unsigned short border_size;
    Color border_color;
} Style;

#endif // !STYLE_H
