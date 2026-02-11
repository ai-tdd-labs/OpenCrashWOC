#include "nuvec_demo.h"

f32 NuVecDot_demo(nuvec_s* a, nuvec_s* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

