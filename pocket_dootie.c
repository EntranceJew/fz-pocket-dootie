#include "cursor.h"
#include "dootie.h"
#include "room.h"
#include <pocket_dootie_icons.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <toolbox/stream/file_stream.h>
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

//region Globals
static uint32_t last_simulation;
//endregion Globals

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

static void load_world_state() {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    Stream* file_stream = file_stream_alloc(storage);
    if(file_stream_open(
           file_stream, APP_DATA_PATH("world_state.txt"), FSAM_READ, FSOM_OPEN_EXISTING)) {
        FuriString* line_string = furi_string_alloc();
        if(stream_read_line(file_stream, line_string)) {
            last_simulation = furi_hal_rtc_get_timestamp();
            sscanf(furi_string_get_cstr(line_string), "%lu", &last_simulation);
        }
        furi_string_free(line_string);
    }

    file_stream_close(file_stream);
    stream_free(file_stream);
    furi_record_close(RECORD_STORAGE);
}

static void save_world_state() {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    Stream* file_stream = file_stream_alloc(storage);

    if(file_stream_open(
           file_stream, APP_DATA_PATH("world_state.txt"), FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        FuriString* line_string = furi_string_alloc_printf("%lu", last_simulation);

        const size_t written = stream_write_string(file_stream, line_string);
        if(written > 0) {
            FURI_LOG_D(TAG, "DATA WAS RECORDED %zu", written);
        } else {
            FURI_LOG_D(TAG, "AINT WROTE SHIT");
        }

        furi_string_free(line_string);
    }
    file_stream_close(file_stream);

    stream_free(file_stream);
    furi_record_close(RECORD_STORAGE);
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

    // LOADING WORLD, TICK-WARP
    FURI_LOG_D(TAG, "starting out: %lu", last_simulation);
    load_world_state();
    const uint32_t tick_warp = furi_hal_rtc_get_timestamp() - last_simulation;
    FURI_LOG_D(TAG, "tick-warping: %lu time!", tick_warp);

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
        // @TODO: skip draws if nothing needs to change?
        view_port_update(view_port);
        last_simulation = furi_hal_rtc_get_timestamp();
    }

    // CLOSE SIMULATION
    save_world_state();

    // shutdown
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);

    furi_record_close(RECORD_GUI);

    return 0;
}
