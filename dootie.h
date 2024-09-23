#pragma once
#include <pocket_dootie_icons.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/icon_i.h>

typedef struct {
    int8_t x, y;
} Point2D;

typedef enum {
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
} DootieAnimationState;

typedef struct {
    Point2D pos;
    DootieAnimationState state;
} Dootie;

typedef struct {
    char* playerName;
    uint32_t currency;
    uint32_t poops;
} DootieWorld;

typedef struct {
    bool alarmEnabled;
    DateTime alarmTime;
} DootieSave;

const Icon* get_dootie_sprite_s(Dootie dootie, uint8_t frame);
const Icon* get_dootie_sprite_f(Dootie dootie, uint8_t frame);
const Icon* get_dootie_sprite_o(Dootie dootie, uint8_t frame);

void dootie_draw(Dootie dootie, Canvas* canvas, uint8_t frame, bool invert);
