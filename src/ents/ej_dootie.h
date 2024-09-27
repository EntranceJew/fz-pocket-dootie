#ifdef EJ_DOOTIE_IMPLEMENTATION
#undef EJ_DOOTIE_IMPLEMENTATION // don't implement more than once

#ifndef EJ_DOOTIE_FURI_TAG
#define EJ_DOOTIE_FURI_TAG          "EJ_DOOTIE"
// this is absolutely perfect
#define POCKET_DOOTIE_APP_EXTENSION ".dootie"
#endif

#include "../ej_math.h"
#define EJ_ROOM_IMPLEMENTATION
#include "../ej_room.h"
#include <pocket_dootie_icons.h>
#include <datetime.h>
#include <gui/canvas.h>
#include <furi_hal_random.h>
#include <core/record.h>
#include <storage/storage.h>
#include <stream/file_stream.h>
#include <stream/stream.h>

typedef enum DootieAnimationState {
    DAS_DEAD,
    DAS_EGG,
    DAS_IDLE,
    DAS_BLINK,
    DAS_NOISE,
    DAS_SIT,
    DAS_SLEEP,
    DAS_BACK,

    // emotions
    DAS_HAPPY,
    DAS_SAD,
    DAS_DEPRESSED,
    DAS_ANGRY,

    // one-shot animations:
    DAS_EAT,
    DAS_POOP,
    DAS_ATTACK,
    // DAS_HATCH,

    _DAS_MAX_VALUE,
} DootieAnimationState;

typedef struct Dootie {
    uint8_t uuid[16];
    Point2D pos;
    DootieAnimationState state;
    uint32_t ticks;
} Dootie;

typedef struct DootieWorld {
    char* playerName;
    uint32_t currency;
    uint32_t poops;
} DootieWorld;

typedef struct DootieSave {
    bool alarmEnabled;
    DateTime alarmTime;
} DootieSave;

const TriBlendFrameSequence dootie_sprites[] = {
    [DAS_DEAD] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_DEAD_01_shape,
                     .fill = &I_dootie_DEAD_01_fill,
                     .outline = &I_dootie_DEAD_01_outline},
                }},
    [DAS_EGG] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_EGG_01_shape,
                     .fill = &I_dootie_EGG_01_fill,
                     .outline = &I_dootie_EGG_01_outline},
                    {.shape = &I_dootie_EGG_02_shape,
                     .fill = &I_dootie_EGG_02_fill,
                     .outline = &I_dootie_EGG_02_outline},
                }},
    [DAS_IDLE] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_IDLE_01_shape,
                     .fill = &I_dootie_IDLE_01_fill,
                     .outline = &I_dootie_IDLE_01_outline},
                    {.shape = &I_dootie_IDLE_02_shape,
                     .fill = &I_dootie_IDLE_02_fill,
                     .outline = &I_dootie_IDLE_02_outline},
                },
        },
    [DAS_BLINK] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_BLINK_01_shape,
                     .fill = &I_dootie_BLINK_01_fill,
                     .outline = &I_dootie_BLINK_01_outline},
                    {.shape = &I_dootie_BLINK_02_shape,
                     .fill = &I_dootie_BLINK_02_fill,
                     .outline = &I_dootie_BLINK_02_outline},
                },
        },
    [DAS_NOISE] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_NOISE_01_shape,
                     .fill = &I_dootie_NOISE_01_fill,
                     .outline = &I_dootie_NOISE_01_outline},
                    {.shape = &I_dootie_NOISE_02_shape,
                     .fill = &I_dootie_NOISE_02_fill,
                     .outline = &I_dootie_NOISE_02_outline},
                },
        },
    [DAS_SIT] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_SIT_01_shape,
                     .fill = &I_dootie_SIT_01_fill,
                     .outline = &I_dootie_SIT_01_outline},
                    {.shape = &I_dootie_SIT_02_shape,
                     .fill = &I_dootie_SIT_02_fill,
                     .outline = &I_dootie_SIT_02_outline},
                },
        },
    [DAS_SLEEP] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_SLEEP_01_shape,
                     .fill = &I_dootie_SLEEP_01_fill,
                     .outline = &I_dootie_SLEEP_01_outline},
                    {.shape = &I_dootie_SLEEP_02_shape,
                     .fill = &I_dootie_SLEEP_02_fill,
                     .outline = &I_dootie_SLEEP_02_outline},
                },
        },
    [DAS_BACK] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_BACK_01_shape,
                     .fill = &I_dootie_BACK_01_fill,
                     .outline = &I_dootie_BACK_01_outline},
                    {.shape = &I_dootie_BACK_02_shape,
                     .fill = &I_dootie_BACK_02_fill,
                     .outline = &I_dootie_BACK_02_outline},
                },
        },
    [DAS_HAPPY] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_HAPPY_01_shape,
                     .fill = &I_dootie_HAPPY_01_fill,
                     .outline = &I_dootie_HAPPY_01_outline},
                    {.shape = &I_dootie_HAPPY_02_shape,
                     .fill = &I_dootie_HAPPY_02_fill,
                     .outline = &I_dootie_HAPPY_02_outline},
                },
        },
    [DAS_SAD] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_SAD_01_shape,
                     .fill = &I_dootie_SAD_01_fill,
                     .outline = &I_dootie_SAD_01_outline},
                    {.shape = &I_dootie_SAD_02_shape,
                     .fill = &I_dootie_SAD_02_fill,
                     .outline = &I_dootie_SAD_02_outline},
                },
        },
    [DAS_DEPRESSED] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_DEPRESSED_01_shape,
                     .fill = &I_dootie_DEPRESSED_01_fill,
                     .outline = &I_dootie_DEPRESSED_01_outline},
                    {.shape = &I_dootie_DEPRESSED_02_shape,
                     .fill = &I_dootie_DEPRESSED_02_fill,
                     .outline = &I_dootie_DEPRESSED_02_outline},
                },
        },
    [DAS_ANGRY] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_ANGRY_01_shape,
                     .fill = &I_dootie_ANGRY_01_fill,
                     .outline = &I_dootie_ANGRY_01_outline},
                    {.shape = &I_dootie_ANGRY_02_shape,
                     .fill = &I_dootie_ANGRY_02_fill,
                     .outline = &I_dootie_ANGRY_02_outline},
                },
        },
    [DAS_EAT] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_EAT_01_shape,
                     .fill = &I_dootie_EAT_01_fill,
                     .outline = &I_dootie_EAT_01_outline},
                    {.shape = &I_dootie_EAT_02_shape,
                     .fill = &I_dootie_EAT_02_fill,
                     .outline = &I_dootie_EAT_02_outline},
                },
        },
    [DAS_POOP] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_POOP_01_shape,
                     .fill = &I_dootie_POOP_01_fill,
                     .outline = &I_dootie_POOP_01_outline},
                    {.shape = &I_dootie_POOP_02_shape,
                     .fill = &I_dootie_POOP_02_fill,
                     .outline = &I_dootie_POOP_02_outline},
                },
        },
    [DAS_ATTACK] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.shape = &I_dootie_EGG_01_shape,
                     .fill = &I_dootie_EGG_01_fill,
                     .outline = &I_dootie_EGG_01_outline},
                    {.shape = &I_dootie_EGG_02_shape,
                     .fill = &I_dootie_EGG_02_fill,
                     .outline = &I_dootie_EGG_02_outline},
                },
        },
};
//</editor-fold>

inline extern void dootie_init(Dootie* dootie) {
    furi_hal_random_fill_buf(dootie->uuid, sizeof(dootie->uuid));
}

inline extern void
    dootie_draw(const Dootie dootie, Canvas* canvas, const uint8_t frame, const bool invert) {
    // const uint8_t even_frame = frame % 2;
    const Point2D draw_point = room_screen_to_world(dootie.pos);

    const TriBlendFrameSequence seq = dootie_sprites[dootie.state];
    const TriBlendFrame tri_blend_frame = seq.frames[frame % seq.num_frames];

    canvas_set_color(canvas, invert ? ColorWhite : ColorBlack);
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, tri_blend_frame.shape);

    canvas_set_color(canvas, invert ? ColorBlack : ColorWhite);
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, tri_blend_frame.fill);

    // add a weird phase to drawing highlighted characters
    if(invert) {
        const bool is_even_sequence = 0 == seq.num_frames % 2;
        canvas_set_color(canvas, frame % (is_even_sequence ? 3 : 2) ? ColorBlack : ColorWhite);
    }
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, tri_blend_frame.outline);
}

inline extern void dootie_tick(Dootie* dootie, const uint64_t ticks) {
    dootie->ticks += ticks;

    const uint64_t time_scale = 1;

    const uint64_t idle = time_scale * 3;
    const uint64_t happy = time_scale * 10;
    const uint64_t sad = time_scale * 15;
    const uint64_t dead = time_scale * 17;

    if(dootie->ticks >= dead) {
        dootie->state = DAS_DEAD;
    } else if(dootie->ticks >= sad) {
        dootie->state = DAS_SAD;
    } else if(dootie->ticks >= happy) {
        dootie->state = DAS_HAPPY;
    } else if(dootie->ticks >= idle) {
        dootie->state = DAS_IDLE;
    } else {
        dootie->state = DAS_EGG;
    }
}

inline extern void dootie_save_to_file(Dootie dootie) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    Stream* file_stream = file_stream_alloc(storage);

    FuriString* dootie_hex_tag = furi_string_alloc();
    for(uint8_t j = 0; j < sizeof(dootie.uuid); ++j) {
        furi_string_cat_printf(dootie_hex_tag, "%02X", dootie.uuid[j]);
    }

    FuriString* absolute_dootie_save_path = furi_string_alloc_printf(
        "%s%s%s",
        APP_DATA_PATH(""),
        furi_string_get_cstr(dootie_hex_tag),
        POCKET_DOOTIE_APP_EXTENSION);
    const char* absolute_dootie_save_path_string = furi_string_get_cstr(absolute_dootie_save_path);

    if(file_stream_open(
           file_stream, absolute_dootie_save_path_string, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        stream_write(file_stream, (uint8_t*)&dootie, sizeof(dootie));
        FURI_LOG_D(
            EJ_DOOTIE_FURI_TAG,
            "writing: %s (%u / %u)",
            absolute_dootie_save_path_string,
            sizeof(dootie),
            sizeof(Dootie));
    } else {
        FURI_LOG_D(EJ_DOOTIE_FURI_TAG, "did not write to %s", absolute_dootie_save_path_string);
    }

    furi_string_free(absolute_dootie_save_path);
    file_stream_close(file_stream);
    stream_free(file_stream);

    furi_string_free(dootie_hex_tag);

    furi_record_close(RECORD_STORAGE);
}

inline extern bool dootie_load_from_file(const char name[], Dootie* dootie) {
    bool ret = false;
    FuriString* full_path = furi_string_alloc_printf("%s%s", APP_DATA_PATH(""), name);
    if(furi_string_end_with_str(full_path, POCKET_DOOTIE_APP_EXTENSION)) {
        Storage* storage = furi_record_open(RECORD_STORAGE);
        Stream* file_stream = file_stream_alloc(storage);

        if(file_stream_open(
               file_stream, furi_string_get_cstr(full_path), FSAM_READ, FSOM_OPEN_EXISTING)) {
            const size_t size = stream_size(file_stream);
            FURI_LOG_I(
                EJ_DOOTIE_FURI_TAG,
                "attempting to load dootie: %s, %i",
                furi_string_get_cstr(full_path),
                size);
            if(stream_read(file_stream, (uint8_t*)dootie, stream_size(file_stream))) {
                /*** IMPORTANT: LOGIC FOR PREVENTING LOADING ***/
                if(DAS_DEAD == dootie->state) {
                    FURI_LOG_I(
                        EJ_DOOTIE_FURI_TAG,
                        "ignoring dead dootie: %s",
                        furi_string_get_cstr(full_path));
                    ret = false;
                } else {
                    ret = true;
                }
            } else {
                FURI_LOG_E(
                    EJ_DOOTIE_FURI_TAG,
                    "could not read dootie: %s",
                    furi_string_get_cstr(full_path));
            }
        } else {
            FURI_LOG_E(
                EJ_DOOTIE_FURI_TAG, "could not open dootie: %s", furi_string_get_cstr(full_path));
        }

        file_stream_close(file_stream);
        stream_free(file_stream);
    }

    furi_string_free(full_path);
    furi_record_close(RECORD_STORAGE);
    return ret;
}

#endif // EJ_DOOTIE_IMPLEMENTATION

#if !defined(INCLUDE_EJ_DOOTIE_H)
#define INCLUDE_EJ_DOOTIE_H

#endif // INCLUDE_EJ_DOOTIE_H
