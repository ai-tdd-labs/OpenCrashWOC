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

extern int Level;
extern int SplTab;
extern int world_scene;

void InitSplineTable(void) {
  s32 i;
  
  if (world_scene[0] != NULL) {
    for (i = 0; i < 0x49; i++) {
      SplTab[i].spl = NULL;
      if (((SplTab[i].levbits >> Level) & 1) != 0) {
        SplTab[i].spl = NuSplineFind(world_scene[0],SplTab[i].name);
        if (SplTab[i].spl != NULL) {
          if ((SplTab[i].min > 0) && (SplTab[i].spl->len < SplTab[i].min)) {
            SplTab[i].spl = NULL;
          }
          else {
            if ((SplTab[i].max > 0) && ((SplTab[i].max >= SplTab[i].min  && (SplTab[i].spl->len > SplTab[i].max)))) {
              SplTab[i].spl = 0;
            }
          }
        }
      }
    }
  }
  else {
    for (i = 0; i < 0x49; i++) {
      SplTab[i].spl = NULL;
    }
  }
  return;
}
