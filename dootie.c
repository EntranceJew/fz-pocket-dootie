#include "dootie.h"
#include <pocket_dootie_icons.h>
#include <room.h>
#include <gui/gui.h>
#include <storage/storage.h>

/**
 * the big normal smelling data table
 *
 * Shape (Silhouette, Interior Lines)
 * Fill ([in]Front Of Thing)
 * Outline (Helps Read Shape, Highlight)
 */

const TriBlendFrameSequence dootie_sprites[] = {
    // NOLINT(*-interfaces-global-init)
    [DAS_DEAD] =
        (TriBlendFrameSequence){
            .num_frames = 1,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_DEAD_01_s, .f = &I_dootie_DEAD_01_f, .o = &I_dootie_DEAD_01_o},
                }},
    [DAS_EGG] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_EGG_01_s, .f = &I_dootie_EGG_01_f, .o = &I_dootie_EGG_01_o},
                    {.s = &I_dootie_EGG_02_s, .f = &I_dootie_EGG_02_f, .o = &I_dootie_EGG_02_o},
                }},
    [DAS_IDLE] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_IDLE_01_s, .f = &I_dootie_IDLE_01_f, .o = &I_dootie_IDLE_01_o},
                    {.s = &I_dootie_IDLE_02_s, .f = &I_dootie_IDLE_02_f, .o = &I_dootie_IDLE_02_o},
                },
        },
    [DAS_BLINK] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_BLINK_01_s,
                     .f = &I_dootie_BLINK_01_f,
                     .o = &I_dootie_BLINK_01_o},
                    {.s = &I_dootie_BLINK_02_s,
                     .f = &I_dootie_BLINK_02_f,
                     .o = &I_dootie_BLINK_02_o},
                },
        },
    [DAS_NOISE] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_NOISE_01_s,
                     .f = &I_dootie_NOISE_01_f,
                     .o = &I_dootie_NOISE_01_o},
                    {.s = &I_dootie_NOISE_02_s,
                     .f = &I_dootie_NOISE_02_f,
                     .o = &I_dootie_NOISE_02_o},
                },
        },
    [DAS_SIT] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_SIT_01_s, .f = &I_dootie_SIT_01_f, .o = &I_dootie_SIT_01_o},
                    {.s = &I_dootie_SIT_02_s, .f = &I_dootie_SIT_02_f, .o = &I_dootie_SIT_02_o},
                },
        },
    [DAS_SLEEP] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_SLEEP_01_s,
                     .f = &I_dootie_SLEEP_01_f,
                     .o = &I_dootie_SLEEP_01_o},
                    {.s = &I_dootie_SLEEP_02_s,
                     .f = &I_dootie_SLEEP_02_f,
                     .o = &I_dootie_SLEEP_02_o},
                },
        },
    [DAS_BACK] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_BACK_01_s, .f = &I_dootie_BACK_01_f, .o = &I_dootie_BACK_01_o},
                    {.s = &I_dootie_BACK_02_s, .f = &I_dootie_BACK_02_f, .o = &I_dootie_BACK_02_o},
                },
        },
    [DAS_HAPPY] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_HAPPY_01_s,
                     .f = &I_dootie_HAPPY_01_f,
                     .o = &I_dootie_HAPPY_01_o},
                    {.s = &I_dootie_HAPPY_02_s,
                     .f = &I_dootie_HAPPY_02_f,
                     .o = &I_dootie_HAPPY_02_o},
                },
        },
    [DAS_SAD] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_SAD_01_s, .f = &I_dootie_SAD_01_f, .o = &I_dootie_SAD_01_o},
                    {.s = &I_dootie_SAD_02_s, .f = &I_dootie_SAD_02_f, .o = &I_dootie_SAD_02_o},
                },
        },
    [DAS_DEPRESSED] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_DEPRESSED_01_s,
                     .f = &I_dootie_DEPRESSED_01_f,
                     .o = &I_dootie_DEPRESSED_01_o},
                    {.s = &I_dootie_DEPRESSED_02_s,
                     .f = &I_dootie_DEPRESSED_02_f,
                     .o = &I_dootie_DEPRESSED_02_o},
                },
        },
    [DAS_ANGRY] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_ANGRY_01_s,
                     .f = &I_dootie_ANGRY_01_f,
                     .o = &I_dootie_ANGRY_01_o},
                    {.s = &I_dootie_ANGRY_02_s,
                     .f = &I_dootie_ANGRY_02_f,
                     .o = &I_dootie_ANGRY_02_o},
                },
        },
    [DAS_EAT] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_EAT_01_s, .f = &I_dootie_EAT_01_f, .o = &I_dootie_EAT_01_o},
                    {.s = &I_dootie_EAT_02_s, .f = &I_dootie_EAT_02_f, .o = &I_dootie_EAT_02_o},
                },
        },
    [DAS_POOP] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_POOP_01_s, .f = &I_dootie_POOP_01_f, .o = &I_dootie_POOP_01_o},
                    {.s = &I_dootie_POOP_02_s, .f = &I_dootie_POOP_02_f, .o = &I_dootie_POOP_02_o},
                },
        },
    [DAS_ATTACK] =
        (TriBlendFrameSequence){
            .num_frames = 2,
            .frames =
                (TriBlendFrame[]){
                    {.s = &I_dootie_EGG_01_s, .f = &I_dootie_EGG_01_f, .o = &I_dootie_EGG_01_o},
                    {.s = &I_dootie_EGG_02_s, .f = &I_dootie_EGG_02_f, .o = &I_dootie_EGG_02_o},
                },
        },
};

void dootie_draw(const Dootie dootie, Canvas* canvas, const uint8_t frame, const bool invert) {
    // const uint8_t even_frame = frame % 2;
    const Point2D draw_point = room_screen_to_world(dootie.pos);

    const TriBlendFrameSequence seq = dootie_sprites[dootie.state];
    const TriBlendFrame tri_blend_frame = seq.frames[frame % seq.num_frames];

    canvas_set_color(canvas, invert ? ColorWhite : ColorBlack);
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, tri_blend_frame.s);

    canvas_set_color(canvas, invert ? ColorBlack : ColorWhite);
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, tri_blend_frame.f);

    // add a weird phase to drawing highlighted characters
    if(invert) {
        const bool is_even_sequence = 0 == seq.num_frames % 2;
        canvas_set_color(canvas, frame % (is_even_sequence ? 3 : 2) ? ColorBlack : ColorWhite);
    }
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, tri_blend_frame.o);
}
