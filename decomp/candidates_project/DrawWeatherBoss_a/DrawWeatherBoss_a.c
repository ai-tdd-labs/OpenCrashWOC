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

extern int Boss;
extern int WBSCALE;
extern int mTEMP;

void DrawWeatherBoss_a(BOSSSTRUCT *Boss) {
  struct nuvec_s Pos;
  
  Pos = (Boss->Position);
  NuMtxSetScale(&mTEMP,SetNuVecPntr(WBSCALE,WBSCALE,WBSCALE));
  NuMtxRotateY(&mTEMP,(Boss->AngleY * 182.04445f));
  NuMtxTranslate(&mTEMP,&Pos);
  if (Boss->Action == 5) {
      Boss->Seen = MyDrawModelNew(&Boss->BonesDraw,&mTEMP,Boss->Locator);
  } else {
       Boss->Seen = MyDrawModelNew(&Boss->MainDraw,&mTEMP,Boss->Locator);
  }
}
