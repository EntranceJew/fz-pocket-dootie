// #include "cursor.h"
#define EJ_MATH_IMPLEMENTATION
#include "src/ej_math.h"
// #define EJ_ROOM_IMPLEMENTATION
// #include "src/ej_room.h"
#define EJ_DOOTIE_IMPLEMENTATION
#include "src/ents/ej_dootie.h"
#define EJ_CURSOR_IMPLEMENTATION
#include "src/ej_cursor.h"

#include <pocket_dootie_icons.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <toolbox/stream/file_stream.h>
#include <storage/storage.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>
#include <lib/toolbox/stream/stream.h>

// hunka bunka radoos
#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

// stops working when made to be 25:
#define QUEUE_TIMEOUT 100
#define QUEUE_COUNT   8
#define TAG           "PocketDootie"

#define ASSET_PIXEL_BUFFER 1
#define DOOTIE_W           16
#define DOOTIE_H           16

#define MAX_NAME_LENGTH 255

// this is bullshit
#define MAX_DOOTIES 16

//region Globals
static uint32_t last_simulation;
static Point2D FLOOR_POINTS[] = {
    // top-left
    {32, 32},
    // top-right
    {65 - DOOTIE_W, 32},
    // bottom-left
    {1, 63 - DOOTIE_H},
    // bottom-right
    {126 - DOOTIE_W, 63 - DOOTIE_H}};
//endregion Globals

//region Starting Data
static uint8_t max_dootie_index;
static Dootie dooties[MAX_DOOTIES];
static Cursor cursor = {
    .pos = {.x = 32, .y = 32},
    .move_speed = {.x = 4, .y = 2},
    .hot_spot_offset = {.x = 9, .y = 10},
    .state = CAS_SELECT,
};
//endregion Starting Data

static uint32_t stopwatch_ticks() {
    const uint32_t now_timestamp = furi_hal_rtc_get_timestamp();
    const uint32_t past_ticks = now_timestamp - (last_simulation == 0 ? now_timestamp : last_simulation);

    last_simulation = now_timestamp;
    return past_ticks;
}

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

    for(i = 0; i < max_dootie_index; ++i) {
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

    for(i = 0; i < max_dootie_index; i++) {
        const Dootie dootie = dooties[i];
        const uint8_t dootie_index = i;
        const uint8_t frame = curr_dt.second + dootie_index;

        dootie_draw(dootie, canvas, frame, closest_dootie_index == dootie_index);
    }

    cursor_draw(cursor, canvas, 1, false);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void app_think_callback(const uint32_t ticks, void* ctx) {
    UNUSED(ctx);

    uint8_t i = 0;
    for(i = 0; i < max_dootie_index; i++) {
        Dootie* dootie = &dooties[i];
        if(dootie->state == DAS_DEAD) {
            continue;
        }
        // const uint8_t dootie_index = i;
        // const uint8_t frame = curr_dt.second + dootie_index;

        dootie_tick(dootie, ticks);
        FURI_LOG_T(
            TAG, "thinking as Dootie#%i: %lu time! (lifetime: %lu)", i, ticks, dootie->ticks);
    }
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void app_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);

    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

static void load_world_state() {
    // load the world
    Storage* storage = furi_record_open(RECORD_STORAGE);
    Stream* file_stream = file_stream_alloc(storage);
    if(file_stream_open(
           file_stream, APP_DATA_PATH("world_state.txt"), FSAM_READ, FSOM_OPEN_EXISTING)) {
        FuriString* line_string = furi_string_alloc();
        if(stream_read_line(file_stream, line_string)) {
            // if last_simulation is nothing:
            // last_simulation = furi_hal_rtc_get_timestamp();
            sscanf(furi_string_get_cstr(line_string), "%lu", &last_simulation);
        }
        furi_string_free(line_string);
    }
    file_stream_close(file_stream);
    stream_free(file_stream);
    furi_record_close(RECORD_STORAGE);

    // load the dooties
    storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);
    FuriString* directory_thus_far = furi_string_alloc_set_str(STORAGE_APP_DATA_PATH_PREFIX);

    const char* gonk = furi_string_get_cstr(directory_thus_far);
    if(storage_dir_open(file, gonk)) {
        FileInfo file_info;
        char name[MAX_NAME_LENGTH];
        bool read_done = false;

        while(storage_dir_read(file, &file_info, name, MAX_NAME_LENGTH)) {
            read_done = true;
            if(file_info_is_dir(&file_info)) {
                continue;
            }

            Dootie dootie;
            if(dootie_load_from_file(name, &dootie) && max_dootie_index < MAX_DOOTIES) {
                dooties[max_dootie_index] = dootie;
                max_dootie_index++;
            } else {
                FURI_LOG_D(TAG, "this dootie grew up wrong... %s", name);
            }
        }
        if(!read_done) {
            FURI_LOG_E(TAG, "there were no dooties!!!");
        }
    } else {
        FURI_LOG_E(TAG, "could not open %p", directory_thus_far);
    }

    furi_string_free(directory_thus_far);
    storage_dir_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
}

static void save_world_state() {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    Stream* file_stream = file_stream_alloc(storage);

    if(file_stream_open(
           file_stream, APP_DATA_PATH("world_state.txt"), FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        FuriString* line_string = furi_string_alloc_printf("%lu", last_simulation);

        const size_t written = stream_write_string(file_stream, line_string);
        if(written <= 0) {
            FURI_LOG_E(
                TAG,
                "WORLD STATE FAILED TO WRITE: %i / %i",
                written,
                furi_string_size(line_string));
        }

        furi_string_free(line_string);
    }
    file_stream_close(file_stream);
    stream_free(file_stream);
    furi_record_close(RECORD_STORAGE);

    // now the dooties
    uint8_t i = 0;
    for(i = 0; i < max_dootie_index; ++i) {
        dootie_save_to_file(dooties[i]);
    }
}

void app_boot_callback(void) {
    // only add dooties if we have no dooties
    if(max_dootie_index <= 0) {
        uint8_t i = 0;
        FURI_LOG_I(TAG, "all your dooties were dead so you get new ones for free!!!");

        max_dootie_index = 3;
        dooties[0] = (Dootie){.pos = {46, 36}, .state = DAS_EGG, .ticks = 0};
        dooties[1] = (Dootie){.pos = {31, 36}, .state = DAS_BACK, .ticks = 0};
        dooties[2] = (Dootie){.pos = {64, 36}, .state = DAS_DEPRESSED, .ticks = 0};
        // (Dootie){.pos = {32, 28}, .state = DAS_IDLE},
        // (Dootie){.pos = {16, 46}, .state = DAS_HAPPY},
        // (Dootie){.pos = {25, 36}, .state = DAS_EGG},
        // (Dootie){.pos = {76, 42}, .state = DAS_DEPRESSED},
        // };

        for(i = 0; i < max_dootie_index; i++) {
            Dootie* dootie = &dooties[i];
            dootie->pos = math_point_within_points(FLOOR_POINTS, SIZEOF(FLOOR_POINTS));

            dootie_init(dootie);
        }

        // always randomize dooties
        FURI_LOG_T(TAG, "CHANGE PLACES!!!");
        for(i = 0; i < max_dootie_index; i++) {
            Dootie* dootie = &dooties[i];
            dootie->pos = math_point_within_points(FLOOR_POINTS, SIZEOF(FLOOR_POINTS));

            FURI_LOG_D(TAG, "dootie%i: now at (%i,%i)", i, dootie->pos.x, dootie->pos.y);
        }
    }
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
int32_t main_app(void* p) {
    UNUSED(p);

    furi_delay_ms(1000);
    FURI_LOG_T(TAG, "We delayed loading to allow plink time to join after launch.");

    FuriMessageQueue* event_queue = furi_message_queue_alloc(QUEUE_COUNT, sizeof(InputEvent));

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, app_draw_callback, view_port);
    view_port_input_callback_set(view_port, app_input_callback, event_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    NotificationApp* notification = furi_record_open(RECORD_NOTIFICATION);

    notification_message_block(notification, &sequence_display_backlight_enforce_on);

    // LOADING WORLD, TICK-WARP
    FURI_LOG_T(TAG, "starting out: %lu", last_simulation);
    load_world_state();
    uint32_t tick_warp = stopwatch_ticks();
    FURI_LOG_T(TAG, "about to tick-warp (boot): %lu time(s)!", tick_warp);
    uint32_t i;
    for(i = 0; i < tick_warp; ++i) {
        app_think_callback(1, p);
    }

    // @TODO: DEVELOPER BOOTSTRAP, IF THIS FUNCTION ISN'T EMPTY WHEN WE GO GOLD, FIRE ALARM
    app_boot_callback();

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

        // @TODO: separate draw logic from the think logic!
        tick_warp = stopwatch_ticks();
        if(tick_warp > 0) {
            FURI_LOG_T(TAG, "about to tick (normal): %lu time(s)!", tick_warp);
            for(i = 0; i < tick_warp; ++i) {
                app_think_callback(1, p);
            }
        }
    }

    // CLOSE SIMULATION
    save_world_state();

    // Return backlight to normal state
    notification_message(notification, &sequence_display_backlight_enforce_auto);

    // shutdown
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);

    furi_record_close(RECORD_GUI);

    return 0;
}
