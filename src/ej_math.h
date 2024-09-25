#ifdef EJ_MATH_IMPLEMENTATION
#undef EJ_MATH_IMPLEMENTATION // don't implement more than once

#ifndef EJ_MATH_FURI_TAG
#define EJ_MATH_FURI_TAG "EJ_MATH"
#endif

#include <gui/icon.h>

typedef struct Point2D {
    int8_t x, y;
} Point2D;

typedef struct TriBlendFrame {
    const Icon* shape;
    const Icon* fill;
    const Icon* outline;
} TriBlendFrame;

typedef struct TriBlendFrameSequence {
    uint8_t num_frames;
    TriBlendFrame* frames;
} TriBlendFrameSequence;

#endif // EJ_MATH_IMPLEMENTATION

#if !defined(INCLUDE_EJ_MATH_H)
#define INCLUDE_EJ_MATH_H

#endif // INCLUDE_EJ_MATH_H
