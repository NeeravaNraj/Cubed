#include <stddef.h>
#include "../../inc/common.h"
#include "../../inc/raylib/raylib.h"
#include "../../inc/raylib/raygui.h"


void GuiWindow(
    Rectangle bounds,
    Vector2 content_size,
    Vector2* scroll,
    void (*draw_content)(Vector2, Vector2)
) {
    Rectangle scissor = {0};
    Rectangle content = { bounds.x, bounds.y, content_size.x, content_size.y };
    bool require_scissor = bounds.width < content_size.x || bounds.height < content_size.y;
    GuiScrollPanel(bounds, NULL, content, scroll, &scissor);

    if (draw_content != NULL) {
        if (require_scissor) {
            BeginScissorMode(scissor.x, scissor.y, scissor.width, scissor.height);
        }

        draw_content(vec2(bounds.x, bounds.y), *scroll);

        if (require_scissor) {
            EndScissorMode();
        }
    }
}
