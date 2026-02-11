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

extern int CurTerr;
extern int TerI;

float NewShadowPlat(struct nuvec_s *ppos,float size) {
  struct nuvec_s pos;
  
  if (CurTerr == NULL) {
    return 2000000.0f;
  }
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    pos = *ppos;
    NewScan(&pos,0,1);
    NewCast(&pos,5.0f);
    NuScratchRelease();
  return pos.y;
}
