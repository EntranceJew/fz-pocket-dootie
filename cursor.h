#pragma once
#include <dootie.h>
#include <input/input.h>

typedef enum CursorAnimationState {
    CAS_SELECT,
    CAS_PET,
    CAS_GRAB,
    CAS_DROP,
    // hmm
    CAS_POINT_R_UP,
    CAS_POINT_R_RIGHT,
    CAS_POINT_R_DOWN,
    CAS_POINT_R_LEFT,
    CAS_POINT_L_UP,
    CAS_POINT_L_RIGHT,
    CAS_POINT_L_DOWN,
    CAS_POINT_L_LEFT,
} CursorAnimationState;

typedef struct Cursor {
    Point2D pos;
    Point2D move_speed;
    Point2D hot_spot_offset;
    CursorAnimationState state;
} Cursor;

int cursor_handle_input_event(Cursor* cursor, InputEvent event);
void cursor_draw(Cursor cursor, Canvas* canvas);
