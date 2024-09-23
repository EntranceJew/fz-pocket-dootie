#include "cursor.h"
#include "furi_hal_resources.h"
#include "input/input.h"

#include <dootie.h>

#define TAG "PocketDootie"

const Icon* get_cursor_sprite_s(Cursor cursor) {
    switch(cursor.state) {
    case CAS_SELECT:
        return &I_cursor_SELECT_01_s;
    case CAS_PET:
        return &I_cursor_PET_01_s;
    case CAS_GRAB:
        return &I_cursor_GRAB_01_s;
    case CAS_DROP:
        return &I_cursor_DROP_01_s;
    case CAS_POINT_R_UP:
        return &I_cursor_POINT_R_UP_01_s;
    case CAS_POINT_R_RIGHT:
        return &I_cursor_POINT_R_RIGHT_01_s;
    case CAS_POINT_R_DOWN:
        return &I_cursor_POINT_R_DOWN_01_s;
    case CAS_POINT_R_LEFT:
        return &I_cursor_POINT_R_LEFT_01_s;
    case CAS_POINT_L_UP:
        return &I_cursor_POINT_L_UP_01_s;
    case CAS_POINT_L_RIGHT:
        return &I_cursor_POINT_L_RIGHT_01_s;
    case CAS_POINT_L_DOWN:
        return &I_cursor_POINT_L_DOWN_01_s;
    case CAS_POINT_L_LEFT:
        return &I_cursor_POINT_L_LEFT_01_s;
    default:
        return NULL;
    }
}

const Icon* get_cursor_sprite_f(Cursor cursor) {
    switch(cursor.state) {
    case CAS_SELECT:
        return &I_cursor_SELECT_01_f;
    case CAS_PET:
        return &I_cursor_PET_01_f;
    case CAS_GRAB:
        return &I_cursor_GRAB_01_f;
    case CAS_DROP:
        return &I_cursor_DROP_01_f;
    case CAS_POINT_R_UP:
        return &I_cursor_POINT_R_UP_01_f;
    case CAS_POINT_R_RIGHT:
        return &I_cursor_POINT_R_RIGHT_01_f;
    case CAS_POINT_R_DOWN:
        return &I_cursor_POINT_R_DOWN_01_f;
    case CAS_POINT_R_LEFT:
        return &I_cursor_POINT_R_LEFT_01_f;
    case CAS_POINT_L_UP:
        return &I_cursor_POINT_L_UP_01_f;
    case CAS_POINT_L_RIGHT:
        return &I_cursor_POINT_L_RIGHT_01_f;
    case CAS_POINT_L_DOWN:
        return &I_cursor_POINT_L_DOWN_01_f;
    case CAS_POINT_L_LEFT:
        return &I_cursor_POINT_L_LEFT_01_f;
    default:
        return NULL;
    }
}

const Icon* get_cursor_sprite_o(Cursor cursor) {
    switch(cursor.state) {
    case CAS_SELECT:
        return &I_cursor_SELECT_01_o;
    case CAS_PET:
        return &I_cursor_PET_01_o;
    case CAS_GRAB:
        return &I_cursor_GRAB_01_o;
    case CAS_DROP:
        return &I_cursor_DROP_01_o;
    case CAS_POINT_R_UP:
        return &I_cursor_POINT_R_UP_01_o;
    case CAS_POINT_R_RIGHT:
        return &I_cursor_POINT_R_RIGHT_01_o;
    case CAS_POINT_R_DOWN:
        return &I_cursor_POINT_R_DOWN_01_o;
    case CAS_POINT_R_LEFT:
        return &I_cursor_POINT_R_LEFT_01_o;
    case CAS_POINT_L_UP:
        return &I_cursor_POINT_L_UP_01_o;
    case CAS_POINT_L_RIGHT:
        return &I_cursor_POINT_L_RIGHT_01_o;
    case CAS_POINT_L_DOWN:
        return &I_cursor_POINT_L_DOWN_01_o;
    case CAS_POINT_L_LEFT:
        return &I_cursor_POINT_L_LEFT_01_o;
    default:
        return NULL;
    }
}

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

void cursor_draw(Cursor cursor, Canvas* canvas) {
    const Icon* graphic = get_cursor_sprite_s(cursor);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, graphic);

    canvas_set_color(canvas, ColorWhite);
    graphic = get_cursor_sprite_f(cursor);
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, graphic);

    graphic = get_cursor_sprite_o(cursor);
    canvas_draw_icon(canvas, cursor.pos.x, cursor.pos.y, graphic);
}
