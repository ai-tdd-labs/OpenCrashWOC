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

extern int CutAudio;
extern int CutInst;
extern int Paused;
extern int cutmovie_hold;
extern int cutworldix;

void UpdateCutMovie(void) {
    if (((cutmovie_hold == 0) || (CutAudio[cutworldix] == -1)) || (NuSoundKeyStatus(4) != 1)) {
        CutInst[cutworldix]->rate = 0.0f;
    }
    else {
        cutmovie_hold = 0;
        SetCutMovieRate();
    }
    NuGCutSceneSysUpdate(Paused);
    return;
}
