#ifdef EJ_ROOM_IMPLEMENTATION
#undef EJ_ROOM_IMPLEMENTATION // don't implement more than once

#ifndef EJ_ROOM_FURI_TAG
#define EJ_ROOM_FURI_TAG "EJ_ROOM"
#endif

#include "ej_math.h"

inline extern Point2D room_screen_to_world(const Point2D pos) {
    return pos;
}

#endif // EJ_ROOM_IMPLEMENTATION

#if !defined(INCLUDE_EJ_ROOM_H)
#define INCLUDE_EJ_ROOM_H

#endif // INCLUDE_EJ_ROOM_H
