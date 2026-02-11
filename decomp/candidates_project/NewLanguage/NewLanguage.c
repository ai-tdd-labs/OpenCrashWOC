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

extern int PData;
extern int tDEATHTORNADOSPINTEXT;
extern int tDOUBLEJUMPTEXT;
extern int tFRUITBAZOOKATEXT;
extern int tSNEAKSHOES;
extern int tSNEAKSHOESTEXT;
extern int tSPEEDSHOESTEXT;
extern int tSUPERBELLYFLOPTEXT;
extern int u8;

void NewLanguage(s32 l) {
  PData[0].name = tSNEAKSHOES[(u8)l];
  PData[0].description = (s32 *)tSNEAKSHOESTEXT[(u8)l];
  PData[1].name = tDOUBLEJUMP[(u8)l];
  PData[1].description = (s32 *)tDOUBLEJUMPTEXT[(u8)l];
  PData[2].name = tDEATHTORNADOSPIN[(u8)l];
  PData[2].description = (s32 *)tDEATHTORNADOSPINTEXT[(u8)l];
  PData[3].name = tFRUITBAZOOKA[(u8)l];
  PData[3].description = (s32 *)tFRUITBAZOOKATEXT[(u8)l];
  PData[4].name = tSPEEDSHOES[(u8)l];
  PData[4].description = (s32 *)tSPEEDSHOESTEXT[(u8)l];
  PData[5].name = tSUPERBELLYFLOP[(u8)l];
  PData[5].description = (s32 *)tSUPERBELLYFLOPTEXT[(u8)l];
  Game.language = l;
  DefaultTimeTrialNames(0);
  return;
}
