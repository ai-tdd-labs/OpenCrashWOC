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

extern int CharacterId;
extern int GUNBOATSTRUCT;
extern int GunBoat;
extern int GunBoatList;

void InitGunBoat(struct nuvec_s *Pos,float AngleY,s32 Character) {
  GUNBOATSTRUCT* GunBoat;
  s32 CharacterId;

  GunBoat = &GunBoatList[Character];
  if (GunBoat->Active != 0) {
    return;
  }
  memset(&GunBoatList[Character],0,0x134);
  GunBoat->Character = Character;
  GunBoat->Position = *Pos;
  GunBoat->AngleY = AngleY;
  GunBoat->Action = 1;
  GunBoat->LastAction = -1;
  GunBoat->FireAngleX = 45.0;
  switch(Character) {
      case 0:
          CharacterId = 0x68;
      break;
      case 1:
          CharacterId = 0x69;
      break;
      case 2:
          CharacterId = 0x67;
      break;
      default:
          CharacterId = 0x66;
      break;
  }
  if (MyInitModelNew(&GunBoat->MainDraw,CharacterId,0x22,0,NULL,&GunBoat->Position) == 0) {
    GunBoat->Character = -1;
  }
  GunBoat->Active = 1;
}
