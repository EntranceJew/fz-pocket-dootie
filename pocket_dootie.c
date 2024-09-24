#include "cursor.h"
#include "dootie.h"
#include "room.h"
#include <pocket_dootie_icons.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <storage/storage.h>

// Screen is 128x64 px
#define SCREEN_X 128
#define SCREEN_Y 64

// stops working when made to be 25:
#define QUEUE_TIMEOUT 100
#define QUEUE_COUNT   8
#define TAG           "PocketDootie"

#define ASSET_PIXEL_BUFFER 1
#define DOOTIE_W           16
#define DOOTIE_H           16

// this is bullshit
#define MAX_DOOTIES 16

//region Starting Data
static uint8_t max_dootie_index = 4;
static Dootie dooties[MAX_DOOTIES] = {
    (Dootie){.pos = {46, 36}, .state = DAS_EAT},
    (Dootie){.pos = {32, 28}, .state = DAS_IDLE},
    (Dootie){.pos = {16, 46}, .state = DAS_HAPPY},
    (Dootie){.pos = {25, 36}, .state = DAS_EGG},
    (Dootie){.pos = {76, 42}, .state = DAS_DEPRESSED}};
static Cursor cursor = {
    .pos = {.x = 32, .y = 32},
    .move_speed = {.x = 4, .y = 2},
    .hot_spot_offset = {.x = 9, .y = 10},
    .state = CAS_SELECT,
};
//endregion Starting Data

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void app_draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);

    DateTime curr_dt;
    furi_hal_rtc_get_datetime(&curr_dt);

    canvas_clear(canvas);
    canvas_set_bitmap_mode(canvas, true);

    // background
    canvas_draw_icon(canvas, 0, 0, &I_room);

    // find the dootie that is nearest being poked
    uint8_t i = 0;
    uint8_t closest_dootie_index = UINT8_MAX;

    const uint8_t finger_x = cursor.pos.x + cursor.hot_spot_offset.x;
    const uint8_t finger_y = cursor.pos.y + cursor.hot_spot_offset.y;

    for(i = 0; i <= max_dootie_index; i++) {
        const Dootie dootie = dooties[i];
        const bool is_touching =
            // righter than left
            finger_x > dootie.pos.x + ASSET_PIXEL_BUFFER
            // below top
            && finger_y > dootie.pos.y + ASSET_PIXEL_BUFFER
            // lefter than right
            && finger_x < dootie.pos.x + ASSET_PIXEL_BUFFER * 2 + DOOTIE_W
            // above bottom
            && finger_y < dootie.pos.y + ASSET_PIXEL_BUFFER * 2 + DOOTIE_H;

        if(is_touching) {
            closest_dootie_index = i;
        }
    }

    for(i = 0; i <= max_dootie_index; i++) {
        const Dootie dootie = dooties[i];
        const uint8_t dootie_index = i;
        const uint8_t frame = curr_dt.second + dootie_index;

        dootie_draw(dootie, canvas, frame, closest_dootie_index == dootie_index);
    }

    cursor_draw(cursor, canvas, 1, false);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void app_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);

    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
int32_t main_app(void* p) {
    UNUSED(p);

    FuriMessageQueue* event_queue = furi_message_queue_alloc(QUEUE_COUNT, sizeof(InputEvent));

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, app_draw_callback, view_port);
    view_port_input_callback_set(view_port, app_input_callback, event_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    InputEvent event;
    while(true) {
        const FuriStatus msg = furi_message_queue_get(event_queue, &event, QUEUE_TIMEOUT);
        if(FuriStatusOk == msg &&
           (InputTypePress == event.type || InputTypeRepeat == event.type)) {
            //handle cursor inputs
            if(-1 == cursor_handle_input_event(&cursor, event)) {
                break;
            }
        }

        // refresh screen
        view_port_update(view_port);
    }

    // shutdown
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);

    furi_record_close(RECORD_GUI);

    return 0;
}
