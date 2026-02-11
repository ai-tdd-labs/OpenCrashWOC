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

extern int ObjTab;
extern int RINGCOUNT;
extern int SplTab;
extern int i_ring;
extern int ring_pos;
extern int ring_xrot;
extern int ring_yrot;
extern int temp_fALONG;
extern int temp_iALONG;
extern int temp_iRAIL;

void ResetRings(void) {
  struct nuvec_s *pos;
  struct nuvec_s v;
  s32 i;
  s32 temp;
  
  RINGCOUNT = 0;
  if (((SplTab[64].spl != NULL) && (ObjTab[103].obj.special != NULL)) && (ObjTab[104].obj.special != NULL)) {
    RINGCOUNT = (s32)(SplTab[64].spl)->len / 2;
    if (0x30 < RINGCOUNT) {
      RINGCOUNT = 0x30;
    }
    pos = (struct nuvec_s *)(SplTab[64].spl)->pts;
    GetALONG(pos,NULL,-1,-1,1);
    if (AheadOfCheckpoint(temp_iRAIL,temp_iALONG,temp_fALONG) != 0) {
      i_ring = 0;
    }
    for (i = 0; i < RINGCOUNT; i++, pos+=2) {
        NuVecSub(&v,(pos + 1),pos);
        ring_yrot[i] = (short) NuAtan2D(v.x,v.z);
        temp = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
        ring_xrot[i] = -(short)temp;
        if (i == i_ring) {
          ring_pos = *pos;
        }
    }
  }
  return;
}
