#include "../inc/ui/editor.h"


void movpltstate_init(MovPltState* state) {
    state->step = 0;
    state->startPos = vec2(0, 0);
    state->endPos = vec2(0, 0);
}


void movpltstate_process_steps(MovPltState* state, Vector2 position) {
    if (state->step == 0) {
        state->startPos = position;
        state->step++;
    } else if (state->step == 1)  {
        state->endPos = position;
        state->step++;
    }
}
