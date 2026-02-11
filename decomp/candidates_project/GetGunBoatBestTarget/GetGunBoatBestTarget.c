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

extern int GameCam;
extern int GunBoatList;
extern int v000;

float GetGunBoatBestTarget(float Best,struct nuvec_s **TargetPos,struct nuvec_s **Vel,s32 *Moving) {
  struct nuvec_s CamDir;
  struct nuvec_s Rel;
  s32 i;
  float Mag;
  float Dot;
  
  NuVecMtxRotate(&CamDir,SetNuVecPntr(0.0f,0.0f,1.0f),&GameCam[0].m);
  for(i = 0; i < 4; i++) {
    if (GunBoatList[i].Active != 0) {
      if (GunBoatList[i].Action != 2) {
        NuVecSub(&Rel,&GunBoatList[i].Position,&GameCam[0].pos);
        Mag = NuVecMag(&Rel);
        if (Mag > 10.0f) {
          NuVecScale(&Rel,&Rel,(1.0f / Mag));
          Dot = DotProduct(&CamDir,&Rel);
          if (Dot > Best) {
            *TargetPos = &GunBoatList[i].Position;
            *Vel = &v000;
            *Moving = 0;
            Best = Dot;
          }
        }
      }
    }
  }
  return Best;
}
