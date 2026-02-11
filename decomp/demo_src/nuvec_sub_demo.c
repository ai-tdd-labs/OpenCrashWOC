#include "nuvec_demo.h"

void NuVecSub_demo(nuvec_s* dest, nuvec_s* a, nuvec_s* b) {
    dest->x = a->x - b->x;
    dest->y = a->y - b->y;
    dest->z = a->z - b->z;
}

