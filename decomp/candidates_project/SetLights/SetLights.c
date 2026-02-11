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

void SetLights(struct nucolour3_s *vCOL0,struct nuvec_s *vDIR0,struct nucolour3_s *vCOL1,struct nuvec_s *vDIR1, struct nucolour3_s *vCOL2,struct nuvec_s *vDIR2,struct nuvec_s *vAMB) {
  NuLightSetDirectionalLights(vDIR0,vCOL0,vDIR1,vCOL1,vDIR2,vCOL2);
  NuLightSetAmbientLight(vAMB);
  return;
}
