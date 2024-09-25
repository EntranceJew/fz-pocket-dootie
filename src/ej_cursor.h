// ej_cursor.h - v0.1 - a virtual cursor - public domain
// LICENSE
//
//   See end of file.

#ifdef EJ_CURSOR_IMPLEMENTATION
#undef EJ_CURSOR_IMPLEMENTATION // don't implement more than once

// if we've already included leakcheck before, undefine the macros
//#ifdef malloc
//#undef malloc
//#undef free
//#undef realloc
//#endif

#ifndef EJ_CURSOR_FURI_TAG
#define EJ_CURSOR_FURI_TAG "EJ_CURSOR"
#endif

#include <gui/canvas.h>
#include <input/input.h>
#include "ents/ej_dootie.h"

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

TriBlendFrameSequence cursor_sprites[] = {
    // NOLINT(*-interfaces-global-init)
    [CAS_SELECT] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_cursor_SELECT_01_shape,
                     .fill = &I_cursor_SELECT_01_fill,
                     .outline = &I_cursor_SELECT_01_outline},
                }},
    [CAS_PET] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_cursor_PET_01_shape,
                     .fill = &I_cursor_PET_01_fill,
                     .outline = &I_cursor_PET_01_outline},
                }},
    [CAS_GRAB] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_cursor_GRAB_01_shape,
                     .fill = &I_cursor_GRAB_01_fill,
                     .outline = &I_cursor_GRAB_01_outline},
                }},
    [CAS_DROP] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_cursor_DROP_01_shape,
                     .fill = &I_cursor_DROP_01_fill,
                     .outline = &I_cursor_DROP_01_outline},
                }},
};

inline extern int cursor_handle_input_event(Cursor* cursor, InputEvent event) {
    switch(event.key) {
    case InputKeyLeft:
        cursor->pos.x -= cursor->move_speed.x;
        break;
    case InputKeyRight:
        cursor->pos.x += cursor->move_speed.x;
        break;
    case InputKeyUp:
        cursor->pos.y -= cursor->move_speed.y;
        break;
    case InputKeyDown:
        cursor->pos.y += cursor->move_speed.y;
        break;
    case InputKeyBack:
        return -1;
    case InputKeyOk:
        return 1;
    default:
        FURI_LOG_E(EJ_CURSOR_FURI_TAG, "you pressed a confusing button");
        break;
    }
    return 0;
}

inline extern void
    cursor_draw(Cursor cursor, Canvas* canvas, const uint8_t frame, const bool invert) {
    // const uint8_t even_frame = frame % 2;
    // const Point2D draw_point = room_screen_to_world(dootie.pos);

    const TriBlendFrameSequence seq = cursor_sprites[cursor.state];
    const TriBlendFrame tri_blend_frame = seq.frames[frame % seq.num_frames];

    canvas_set_color(canvas, invert ? ColorWhite : ColorBlack);
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, tri_blend_frame.shape);

    canvas_set_color(canvas, invert ? ColorBlack : ColorWhite);
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, tri_blend_frame.fill);

    // add a weird phase to drawing highlighted characters
    if(invert) {
        const bool is_even_sequence = 0 == seq.num_frames % 2;
        canvas_set_color(canvas, frame % (is_even_sequence ? 3 : 2) ? ColorBlack : ColorWhite);
    }
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, tri_blend_frame.outline);

    // @TODO: Draw dropshadow if carrying things
}
#endif // EJ_CURSOR_IMPLEMENTATION

#if !defined(INCLUDE_EJ_CURSOR_H)
#define INCLUDE_EJ_CURSOR_H

#endif // INCLUDE_EJ_CURSOR_H

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
