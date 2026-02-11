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

extern int BigGunIndx;
extern int BigGunList;

void InitBigGuns(void) {
  memset(BigGunList,0,0xff0);
  BigGunIndx = 0;
  InitBigGun(SetNuVecPntr(0.0,18.86,30.58),0.0,0,-65.0,-10.0,0.0,60.0);
  InitBigGun(SetNuVecPntr(0.0,-1.68,16.72),0.0,4,10.0,80.0,0.0,60.0);
  InitBigGun(SetNuVecPntr(-10.83,6.32,34.15),0.0,5,-65.0,-10.0,-90.0,60.0);
  InitBigGun(SetNuVecPntr(10.83,6.32,34.15),0.0,5,-65.0,-10.0,90.0,60.0);
  InitBigGun(SetNuVecPntr(0.03,23.71,2.48),0.0,0,-65.0,-10.0,180.0,60.0);
  InitBigGun(SetNuVecPntr(0.0,-25.5,-5.97),0.0,3,-30.0,30.0,180.0,60.0);
  InitBigGun(SetNuVecPntr(10.0,9.89,14.46),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(9.5,9.89,11.25),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(9.0,9.89,7.84),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(-10.0,9.89,14.46),0.0,2,-30.0,30.0,-90.0,30.0);
  InitBigGun(SetNuVecPntr(-9.5,9.89,11.25),0.0,2,-30.0,30.0,-90.0,30.0);
  InitBigGun(SetNuVecPntr(-9.0,9.89,7.84),0.0,2,-30.0,30.0,-90.0,30.0);
}
