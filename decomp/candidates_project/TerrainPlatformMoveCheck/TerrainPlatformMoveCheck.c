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

s32 TerrainPlatformMoveCheck(struct nuvec_s *dest,struct nuvec_s *norm,s32 platid,s32 calcimpact,s32 leaveoffonfail) {
  short *CurData;
  struct nuvec_s curvel; // 0x20(r31)
  short hitcnt;
  short hitterrno;
  short hittype;
  short plathit;
  float hittime;
  struct nuvec_s hitnorm; // 0x40(r31)
  struct nuvec_s uhitnorm; // 0x50(r31)
  s32 terrid;
  
  terrid = CurTerr->platdata[platid].terrno;
  CurData = TerI->PlatScanStart;
  while ( 0 < *CurData) {
    if (CurData[1] == terrid) {
        *CurData = (*CurData <= 0 ? *CurData  : -*CurData);
    }
      CurData += (*CurData >= 0 ? *CurData  : -*CurData) * 2 + 2;
  }
  curvel = TerI->curvel;
  hitcnt = TerI->hitcnt;
  hitterrno = TerI->hitterrno;
  hittype = TerI->hittype;
  plathit = TerI->plathit;
  hittime = TerI->hittime;
  hitnorm = TerI->hitnorm;
  uhitnorm = TerI->uhitnorm;
  (TerI->curvel).x = dest->x - (TerI->curpos).x;
  (TerI->curvel).y = dest->y - (TerI->curpos).y;
  (TerI->curvel).z = dest->z - (TerI->curpos).z;
  DerotateMovementVector();
  HitTerrain();
  if ((calcimpact != 0) && (TerI->hittype != 0)) {
    TerrainImpactNorm();
    TerrainImpactPlatform(TerI->flags);
    if (norm != NULL) {
      *norm = TerI->hitnorm;
    }
    *dest = TerI->curpos;
  }
  (TerI->curvel) = curvel;
  TerI->hitcnt = hitcnt;
  TerI->hitterrno = hitterrno;
  TerI->plathit = plathit;
  TerI->hittime = hittime;
  TerI->hitnorm = hitnorm;
  TerI->uhitnorm = uhitnorm;
  CurData = TerI->PlatScanStart;
  if ((TerI->hittype < 0x10) || (leaveoffonfail == 0)) {
    while (*CurData != 0)  {
      if (CurData[1] == terrid) {
          *CurData = -(*CurData <= 0 ? *CurData  : -*CurData);
      }
      CurData += (*CurData >= 0 ? *CurData  : -*CurData) * 2 + 2;
    }
  }
  if (TerI->hittype != 0) {
    TerI->hittype = hittype;
    return 0;
  }
  else {
    TerI->hittype = hittype;
    return 1;
  }
}
