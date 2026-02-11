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

extern int GS_ForceNoAlphaCompareFlag;
extern int GX_ALWAYS;
extern int GX_AOP_OR;
extern int u8;

void GS_SetAlphaCompare(int Func,int Ref) {
  if (GS_ForceNoAlphaCompareFlag != 0) {
    GXSetAlphaCompare(GX_ALWAYS,0,GX_AOP_OR,GX_ALWAYS,0);
  }
  else {
    GXSetAlphaCompare(Func,(u8)Ref,GX_AOP_OR,Func,(u8)Ref);
  }
  return;
}
