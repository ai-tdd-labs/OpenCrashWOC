typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef unsigned int uint;
typedef int bool32;
#ifndef NULL
#define NULL ((void*)0)
#endif

struct nuvec_s { f32 x; f32 y; f32 z; };
struct nuvec4_s { f32 x; f32 y; f32 z; f32 w; };
struct nuinstance_s;
struct nugscn_s;
struct nugobj_s;
struct creature_s;
struct creatcmd_s;
struct numtx_s;
struct nuwindgrp_s;

extern int Wumpa;
extern int temp_ratio;

s32 WumpaRayCast(struct nuvec_s* p0, struct nuvec_s* p1, float ratio0) {
    struct wumpa_s* wmp;
    struct nuvec_s min;
    struct nuvec_s max;
    struct nuvec_s ray;
    struct nuvec_s v;
    float ratio;
    float dp_ray;
    float dp0;
    float dp;
    float r;
    float r2;
    s32 i;

    ratio = ratio0;
    
    min.x = (p0->x < p1->x) ? p0->x : p1->x;
    min.y = (p0->y < p1->y) ? p0->y : p1->y;
    min.z = (p0->z < p1->z) ? p0->z : p1->z;

    max.x = (p0->x > p1->x) ? p0->x : p1->x;
    max.y = (p0->y > p1->y) ? p0->y : p1->y;
    max.z = (p0->z > p1->z) ? p0->z : p1->z;

    NuVecSub(&ray, p1, p0);

    dp = NuVecDot(&ray, &ray);
    dp_ray = dp;
    
    wmp = Wumpa;
    
    r = 0.2f;
    r2 = r * r;
    
    for (i = 0; i < 0x140; i++, wmp++) {
        if ((wmp->active == 2) && (wmp->in_range != 0)) {
            if (((wmp->pos.x + r) < min.x) || ((wmp->pos.z + r) < min.z)) {
                continue;
            }
            if (((wmp->pos.x - r) > max.x) || ((wmp->pos.z - r) > max.z)) {
                continue;
            }
            if (((wmp->pos.y + r) < min.y) || ((wmp->pos.y - r) > max.y)) {
                continue;
            }
            NuVecSub(&v, &wmp->pos, p0);
            dp = NuVecDot(&ray, &v);
            if ((dp >= 0.0f) && (dp <= dp_ray)) {
                dp0 = (dp / dp_ray);
                if (dp0 < ratio) {
                    v.x = ((p1->x - p0->x) * dp0 + p0->x);
                    v.y = ((p1->y - p0->y) * dp0 + p0->y);
                    v.z = ((p1->z - p0->z) * dp0 + p0->z);
                    NuVecSub(&v, &wmp->pos, &v);
                    if ((v.x * v.x + v.y * v.y + v.z * v.z) < r2) {
                        ratio = dp0;
                    }
                }
            }
        }
    }
    temp_ratio = ratio;
    return ratio < 1.0f;
}
