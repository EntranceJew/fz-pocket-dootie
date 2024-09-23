#include "dootie.h"
#include <pocket_dootie_icons.h>
#include <furi.h>
#include <room.h>
#include <stdlib.h>
#include <string.h>
#include <gui/gui.h>
#include <storage/storage.h>

/**
 * the really fucking sickly and disgusting data tables
 *
 * TODO: if you find out how to have a double-indexed thing with enum access & variable elements,
 *  define it as that please for all that is holy
 *
 * Shape (Silhouette, Interior Lines)
 * Fill ([in]Front Of Thing)
 * Outline (Helps Read Shape, Highlight)
 */
const Icon* DAS_ANIM_DEAD_S[] = {&I_dootie_DEAD_01_s};
const Icon* DAS_ANIM_DEAD_F[] = {&I_dootie_DEAD_01_f};
const Icon* DAS_ANIM_DEAD_O[] = {&I_dootie_DEAD_01_o};
const Icon* DAS_ANIM_EGG_S[] = {&I_dootie_EGG_01_s, &I_dootie_EGG_02_s};
const Icon* DAS_ANIM_EGG_F[] = {&I_dootie_EGG_01_f, &I_dootie_EGG_02_f};
const Icon* DAS_ANIM_EGG_O[] = {&I_dootie_EGG_01_o, &I_dootie_EGG_02_o};
const Icon* DAS_ANIM_IDLE_S[] = {&I_dootie_IDLE_01_s, &I_dootie_IDLE_02_s};
const Icon* DAS_ANIM_IDLE_F[] = {&I_dootie_IDLE_01_f, &I_dootie_IDLE_02_f};
const Icon* DAS_ANIM_IDLE_O[] = {&I_dootie_IDLE_01_o, &I_dootie_IDLE_02_o};
const Icon* DAS_ANIM_BLINK_S[] = {&I_dootie_BLINK_01_s, &I_dootie_BLINK_02_s};
const Icon* DAS_ANIM_BLINK_F[] = {&I_dootie_BLINK_01_f, &I_dootie_BLINK_02_f};
const Icon* DAS_ANIM_BLINK_O[] = {&I_dootie_BLINK_01_o, &I_dootie_BLINK_02_o};
const Icon* DAS_ANIM_NOISE_S[] = {&I_dootie_NOISE_01_s, &I_dootie_NOISE_02_s};
const Icon* DAS_ANIM_NOISE_F[] = {&I_dootie_NOISE_01_f, &I_dootie_NOISE_02_f};
const Icon* DAS_ANIM_NOISE_O[] = {&I_dootie_NOISE_01_o, &I_dootie_NOISE_02_o};
const Icon* DAS_ANIM_SIT_S[] = {&I_dootie_SIT_01_s, &I_dootie_SIT_02_s};
const Icon* DAS_ANIM_SIT_F[] = {&I_dootie_SIT_01_f, &I_dootie_SIT_02_f};
const Icon* DAS_ANIM_SIT_O[] = {&I_dootie_SIT_01_o, &I_dootie_SIT_02_o};
const Icon* DAS_ANIM_SLEEP_S[] = {&I_dootie_SLEEP_01_s, &I_dootie_SLEEP_02_s};
const Icon* DAS_ANIM_SLEEP_F[] = {&I_dootie_SLEEP_01_f, &I_dootie_SLEEP_02_f};
const Icon* DAS_ANIM_SLEEP_O[] = {&I_dootie_SLEEP_01_o, &I_dootie_SLEEP_02_o};
const Icon* DAS_ANIM_BACK_S[] = {&I_dootie_BACK_01_s, &I_dootie_BACK_02_s};
const Icon* DAS_ANIM_BACK_F[] = {&I_dootie_BACK_01_f, &I_dootie_BACK_02_f};
const Icon* DAS_ANIM_BACK_O[] = {&I_dootie_BACK_01_o, &I_dootie_BACK_02_o};
const Icon* DAS_ANIM_HAPPY_S[] = {&I_dootie_HAPPY_01_s, &I_dootie_HAPPY_02_s};
const Icon* DAS_ANIM_HAPPY_F[] = {&I_dootie_HAPPY_01_f, &I_dootie_HAPPY_02_f};
const Icon* DAS_ANIM_HAPPY_O[] = {&I_dootie_HAPPY_01_o, &I_dootie_HAPPY_02_o};
const Icon* DAS_ANIM_SAD_S[] = {&I_dootie_SAD_01_s, &I_dootie_SAD_02_s};
const Icon* DAS_ANIM_SAD_F[] = {&I_dootie_SAD_01_f, &I_dootie_SAD_02_f};
const Icon* DAS_ANIM_SAD_O[] = {&I_dootie_SAD_01_o, &I_dootie_SAD_02_o};
const Icon* DAS_ANIM_DEPRESSED_S[] = {&I_dootie_DEPRESSED_01_s, &I_dootie_DEPRESSED_02_s};
const Icon* DAS_ANIM_DEPRESSED_F[] = {&I_dootie_DEPRESSED_01_f, &I_dootie_DEPRESSED_02_f};
const Icon* DAS_ANIM_DEPRESSED_O[] = {&I_dootie_DEPRESSED_01_o, &I_dootie_DEPRESSED_02_o};
const Icon* DAS_ANIM_ANGRY_S[] = {&I_dootie_ANGRY_01_s, &I_dootie_ANGRY_02_s};
const Icon* DAS_ANIM_ANGRY_F[] = {&I_dootie_ANGRY_01_f, &I_dootie_ANGRY_02_f};
const Icon* DAS_ANIM_ANGRY_O[] = {&I_dootie_ANGRY_01_o, &I_dootie_ANGRY_02_o};
const Icon* DAS_ANIM_EAT_S[] = {&I_dootie_EAT_01_s, &I_dootie_EAT_02_s};
const Icon* DAS_ANIM_EAT_F[] = {&I_dootie_EAT_01_f, &I_dootie_EAT_02_f};
const Icon* DAS_ANIM_EAT_O[] = {&I_dootie_EAT_01_o, &I_dootie_EAT_02_o};
const Icon* DAS_ANIM_POOP_S[] = {&I_dootie_POOP_01_s, &I_dootie_POOP_02_s};
const Icon* DAS_ANIM_POOP_F[] = {&I_dootie_POOP_01_f, &I_dootie_POOP_02_f};
const Icon* DAS_ANIM_POOP_O[] = {&I_dootie_POOP_01_o, &I_dootie_POOP_02_o};
const Icon* DAS_ANIM_ATTACK_S[] = {&I_dootie_ATTACK_01_s, &I_dootie_ATTACK_02_s};
const Icon* DAS_ANIM_ATTACK_F[] = {&I_dootie_ATTACK_01_f, &I_dootie_ATTACK_02_f};
const Icon* DAS_ANIM_ATTACK_O[] = {&I_dootie_ATTACK_01_o, &I_dootie_ATTACK_02_o};

const Icon* get_dootie_sprite_s(const Dootie dootie, const uint8_t frame) {
    switch(dootie.state) {
    case DAS_DEAD:
        return DAS_ANIM_DEAD_S[frame];
    case DAS_EGG:
        return DAS_ANIM_EGG_S[frame];
    case DAS_IDLE:
        return DAS_ANIM_IDLE_S[frame];
    case DAS_BLINK:
        return DAS_ANIM_BLINK_S[frame];
    case DAS_NOISE:
        return DAS_ANIM_NOISE_S[frame];
    case DAS_SIT:
        return DAS_ANIM_SIT_S[frame];
    case DAS_SLEEP:
        return DAS_ANIM_SLEEP_S[frame];
    case DAS_BACK:
        return DAS_ANIM_BACK_S[frame];
    case DAS_HAPPY:
        return DAS_ANIM_HAPPY_S[frame];
    case DAS_SAD:
        return DAS_ANIM_SAD_S[frame];
    case DAS_DEPRESSED:
        return DAS_ANIM_DEPRESSED_S[frame];
    case DAS_ANGRY:
        return DAS_ANIM_ANGRY_S[frame];
    case DAS_EAT:
        return DAS_ANIM_EAT_S[frame];
    case DAS_POOP:
        return DAS_ANIM_POOP_S[frame];
    case DAS_ATTACK:
        return DAS_ANIM_ATTACK_S[frame];
    default:
        return NULL;
    }
}

const Icon* get_dootie_sprite_f(const Dootie dootie, const uint8_t frame) {
    switch(dootie.state) {
    case DAS_DEAD:
        return DAS_ANIM_DEAD_F[frame];
    case DAS_EGG:
        return DAS_ANIM_EGG_F[frame];
    case DAS_IDLE:
        return DAS_ANIM_IDLE_F[frame];
    case DAS_BLINK:
        return DAS_ANIM_BLINK_F[frame];
    case DAS_NOISE:
        return DAS_ANIM_NOISE_F[frame];
    case DAS_SIT:
        return DAS_ANIM_SIT_F[frame];
    case DAS_SLEEP:
        return DAS_ANIM_SLEEP_F[frame];
    case DAS_BACK:
        return DAS_ANIM_BACK_F[frame];
    case DAS_HAPPY:
        return DAS_ANIM_HAPPY_F[frame];
    case DAS_SAD:
        return DAS_ANIM_SAD_F[frame];
    case DAS_DEPRESSED:
        return DAS_ANIM_DEPRESSED_F[frame];
    case DAS_ANGRY:
        return DAS_ANIM_ANGRY_F[frame];
    case DAS_EAT:
        return DAS_ANIM_EAT_F[frame];
    case DAS_POOP:
        return DAS_ANIM_POOP_F[frame];
    case DAS_ATTACK:
        return DAS_ANIM_ATTACK_F[frame];
    default:
        return NULL;
    }
}

const Icon* get_dootie_sprite_o(const Dootie dootie, const uint8_t frame) {
    switch(dootie.state) {
    case DAS_DEAD:
        return DAS_ANIM_DEAD_O[frame];
    case DAS_EGG:
        return DAS_ANIM_EGG_O[frame];
    case DAS_IDLE:
        return DAS_ANIM_IDLE_O[frame];
    case DAS_BLINK:
        return DAS_ANIM_BLINK_O[frame];
    case DAS_NOISE:
        return DAS_ANIM_NOISE_O[frame];
    case DAS_SIT:
        return DAS_ANIM_SIT_O[frame];
    case DAS_SLEEP:
        return DAS_ANIM_SLEEP_O[frame];
    case DAS_BACK:
        return DAS_ANIM_BACK_O[frame];
    case DAS_HAPPY:
        return DAS_ANIM_HAPPY_O[frame];
    case DAS_SAD:
        return DAS_ANIM_SAD_O[frame];
    case DAS_DEPRESSED:
        return DAS_ANIM_DEPRESSED_O[frame];
    case DAS_ANGRY:
        return DAS_ANIM_ANGRY_O[frame];
    case DAS_EAT:
        return DAS_ANIM_EAT_O[frame];
    case DAS_POOP:
        return DAS_ANIM_POOP_O[frame];
    case DAS_ATTACK:
        return DAS_ANIM_ATTACK_O[frame];
    default:
        return NULL;
    }
}

void dootie_draw(const Dootie dootie, Canvas* canvas, const uint8_t frame, const bool invert) {
    const uint8_t even_frame = frame % 2;
    const Point2D draw_point = room_screen_to_world(dootie.pos);

    const Icon* graphic = get_dootie_sprite_s(dootie, even_frame);
    canvas_set_color(canvas, (invert ? ColorWhite : ColorBlack));
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, graphic);

    canvas_set_color(canvas, (invert ? ColorBlack : ColorWhite));
    graphic = get_dootie_sprite_f(dootie, even_frame);
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, graphic);

    // add a weird phase to drawing highlighted characters
    if(invert) {
        canvas_set_color(canvas, (frame % 3 ? ColorBlack : ColorWhite));
    }
    graphic = get_dootie_sprite_o(dootie, even_frame);
    canvas_draw_icon(canvas, draw_point.x, draw_point.y, graphic);
}
