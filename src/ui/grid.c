#include "../inc/ui.h"
#include "../inc/common.h"
#include "../inc/raylib/raylib.h"
#include <math.h>
#include <stdio.h>

void grid_render() {
    int spacing = TILE_SIZE;
    int subdivisions = TILE_SIZE / 2;
    float width = GetScreenWidth() - SIDEBAR_WIDTH;
    float height = GetScreenHeight() - TILESELECTOR_HEIGHT;
    // Get the visible area in world space
    Vector2 screenStart = GetScreenToWorld2D((Vector2){0, 0}, camera);
    Vector2 screenEnd = GetScreenToWorld2D(vec2(width, height), camera);

    // Calculate the visible grid bounds
    float startX = floorf(screenStart.x / spacing) * spacing;
    float startY = floorf(screenStart.y / spacing) * spacing;
    float endX = ceilf(screenEnd.x / spacing) * spacing;
    float endY = ceilf(screenEnd.y / spacing) * spacing;

    // Calculate alpha based on zoom level for fade effect
    float alpha = fminf(1.0f, (camera.zoom - 0.3f) / 0.7f);
    if (alpha <= 0.0f) return;

    // Draw vertical lines
    for (float x = startX; x <= endX; x += spacing) {
        // Main grid lines
        Color lineColor = (int)(x / spacing) % subdivisions == 0 ? 
            (Color){80, 80, 80, (unsigned char)(alpha * 255)} :  // Major lines
            (Color){60, 60, 60, (unsigned char)(alpha * 155)};   // Minor lines
        
        DrawLineV(
            (Vector2){x, startY},
            (Vector2){x, endY},
            lineColor
        );
    }

    // Draw horizontal lines
    for (float y = startY; y <= endY; y += spacing) {
        // Main grid lines
        Color lineColor = (int)(y / spacing) % subdivisions == 0 ? 
            (Color){80, 80, 80, (unsigned char)(alpha * 255)} :  // Major lines
            (Color){60, 60, 60, (unsigned char)(alpha * 155)};   // Minor lines

        DrawLineV(
            (Vector2){startX, y},
            (Vector2){endX, y},
            lineColor
        );
    }

    float thickness = 2;
    // Draw coordinate axes
    DrawLineEx(vec2(0, startY), vec2(0, endY), thickness, RED);
    DrawLineEx(vec2(startX, 0), vec2(endX, 0), thickness, GREEN);
}

