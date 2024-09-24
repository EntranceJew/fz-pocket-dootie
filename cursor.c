#include "cursor.h"
#include "furi_hal_resources.h"
#include "input/input.h"

#include <dootie.h>

#define TAG "PocketDootie"

const TriBlendFrameSequence cursor_sprites[] = {
    // NOLINT(*-interfaces-global-init)
    [CAS_SELECT] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_cursor_SELECT_01_s,
                     .f = &I_cursor_SELECT_01_f,
                     .o = &I_cursor_SELECT_01_o},
                }},
    [CAS_PET] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_cursor_PET_01_s, .f = &I_cursor_PET_01_f, .o = &I_cursor_PET_01_o},
                }},
    [CAS_GRAB] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_cursor_GRAB_01_s, .f = &I_cursor_GRAB_01_f, .o = &I_cursor_GRAB_01_o},
                }},
    [CAS_DROP] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_cursor_DROP_01_s, .f = &I_cursor_DROP_01_f, .o = &I_cursor_DROP_01_o},
                }},
};

int cursor_handle_input_event(Cursor* cursor, InputEvent event) {
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
        FURI_LOG_E(TAG, "you pressed a confusing button");
        break;
    }
    return 0;
}

void cursor_draw(Cursor cursor, Canvas* canvas, const uint8_t frame, const bool invert) {
    // const uint8_t even_frame = frame % 2;
    // const Point2D draw_point = room_screen_to_world(dootie.pos);

    const TriBlendFrameSequence seq = cursor_sprites[cursor.state];
    const TriBlendFrame tri_blend_frame = seq.frames[frame % seq.num_frames];

    canvas_set_color(canvas, invert ? ColorWhite : ColorBlack);
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, tri_blend_frame.s);

    canvas_set_color(canvas, invert ? ColorBlack : ColorWhite);
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, tri_blend_frame.f);

    // add a weird phase to drawing highlighted characters
    if(invert) {
        const bool is_even_sequence = 0 == seq.num_frames % 2;
        canvas_set_color(canvas, frame % (is_even_sequence ? 3 : 2) ? ColorBlack : ColorWhite);
    }
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, tri_blend_frame.o);

    // @TODO: Draw dropshadow if carrying things
}
