#include "nuvec_demo.h"

void NuVecScale_demo(nuvec_s* dest, nuvec_s* a, f32 s) {
    dest->x = a->x * s;
    dest->y = a->y * s;
    dest->z = a->z * s;
}

