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

extern int sysgobj;
extern int sysinit;

void NuGobjClose(void) {

    struct nugobj_s* nextobj;

    if ((sysinit != 0) && (sysgobj != NULL))
    {
        do {
            nextobj = sysgobj->sysnext;
            NuGobjDestroy(sysgobj);
            sysgobj = nextobj;
        } while (nextobj != NULL);
    }
    return;
}
