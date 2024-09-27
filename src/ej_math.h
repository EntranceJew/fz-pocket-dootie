#ifdef EJ_MATH_IMPLEMENTATION
#undef EJ_MATH_IMPLEMENTATION // don't implement more than once

#ifndef REMAP
#define REMAP(a1, a2, b1, b2, s) (b1 + (s - a1) * (b2 - b1) / (a2 - a1))
#endif

#ifndef EJ_MATH_FURI_TAG
#define EJ_MATH_FURI_TAG "EJ_MATH"
#endif

#include <stdlib.h>
#include <gui/icon.h>

typedef struct Point2D {
    int8_t x, y;
} Point2D;

typedef struct Point3D {
    int8_t x, y, z;
} Point3D;

typedef struct TriBlendFrame {
    const Icon* shape;
    const Icon* fill;
    const Icon* outline;
} TriBlendFrame;

typedef struct TriBlendFrameSequence {
    uint8_t num_frames;
    TriBlendFrame* frames;
} TriBlendFrameSequence;

// Screen is 128x64 px
#define SCREEN_X 128
#define SCREEN_Y 64

#define SPRITE_W 16
#define SPRITE_H 16

inline extern Point2D math_point_within_points(const Point2D* points, const size_t arr_size) {
    int8_t min_x = INT8_MAX;
    int8_t min_y = INT8_MAX;
    int8_t max_x = INT8_MIN;
    int8_t max_y = INT8_MIN;

    for(unsigned i = 0; i < arr_size; ++i) {
        Point2D point = points[i];

        min_x = MIN(min_x, point.x);
        min_y = MIN(min_y, point.y);
        max_x = MAX(max_x, point.x);
        max_y = MAX(max_y, point.y);
    }

    const int8_t delta_x = abs(max_x - min_x);
    const int8_t delta_y = abs(max_y - min_y);

    return (Point2D){
        .x = min_x + (rand() % delta_x),
        .y = min_y + (rand() % delta_y),
    };
}

#endif // EJ_MATH_IMPLEMENTATION

#if !defined(INCLUDE_EJ_MATH_H)
#define INCLUDE_EJ_MATH_H

#endif // INCLUDE_EJ_MATH_H
