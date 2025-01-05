#ifndef UI_H
#define UI_H

#include "raylib/raylib.h"

#define SIDEBAR_WIDTH 250
#define TILESELECTOR_HEIGHT 160

// Ui Componenets

// Tile selector
void tileselector_init();
void tileselector_render();
bool tileselector_hovered(Vector2 pos);

// Sidebar
void sidebar_render();
bool sidebar_hovered(Vector2 pos);

// Grid lines
void grid_render();

// File viewer
void fileviewer_init(const char* path, void (*onclick)(const char*));
void fileviewer_render();
void fileviewer_deinit();

// Level name editors
void levelnameinput_init();
void levelnameinput_render();
void levelnameinput_deinit();

// UI elements
void GuiWindow(Rectangle bounds, Vector2 content_size, Vector2* scroll, void (*draw_content)(Vector2, Vector2));
void GuiWindowFloating(Vector2 *position, Vector2 *size, bool *minimized, bool *moving, bool *resizing, void (*draw_content)(Vector2, Vector2), Vector2 content_size, Vector2 *scroll, const char* title);
#endif // !UI_H
