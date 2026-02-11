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

extern int Glider;
extern int GliderCollPoints;
extern int Lines;

static void GenerateGliderLines(GLIDERSTRUCT *Glider,struct nuvec_s *Lines) {
  struct numtx_s Mat;
  s32 i;
  
  NuMtxSetRotationZ(&Mat,(Glider->TiltZ * 182.04445f));
  NuMtxRotateX(&Mat,((Glider->TiltX + 15.0f) * 182.04445f));
  NuMtxRotateY(&Mat,(Glider->AngleY * 182.04445f));
  NuMtxTranslate(&Mat,&Glider->Position);
  for(i = 0; i < 5; i++) {
    NuVecMtxTransform(&Lines[i],&GliderCollPoints[i],&Mat);
  }
  Lines[5] = *Lines;
}
