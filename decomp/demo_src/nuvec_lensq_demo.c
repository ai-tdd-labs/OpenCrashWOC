#include "nuvec_demo.h"

f32 NuVecLenSq_demo(nuvec_s* a) {
    return a->x * a->x + a->y * a->y + a->z * a->z;
}

