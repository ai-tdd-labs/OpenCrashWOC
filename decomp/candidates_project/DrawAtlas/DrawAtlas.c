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

extern int CModel;
extern int CRemap;

void DrawAtlas(struct creature_s *Cre) {
  struct ATLASSTRUCT *Atlas;
  struct numtx_s Matrix;
  s32 i;
  
  Atlas = (struct ATLASSTRUCT *)Cre->Buggy;
  NuQuatToMtx(&Atlas->Quat,&Matrix);
  NuMtxTranslate(&Matrix,&Atlas->Position);
  i = CRemap[83];
  if (i != -1) {
    NuHGobjRndr(CModel[i].hobj,&Matrix,1,NULL);
  }
  return;
}
