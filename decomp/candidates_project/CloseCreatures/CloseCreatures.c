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
extern int Character;

typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned int uint;
typedef int bool32;

void CloseCreatures(void) {
  s32 i;
  
  for(i = 0; i < 9; i++) {
        if (Character[i].used != 0) {
          RemoveCreature(&Character[i]);
        }
  }
  for(i = 0; i < 0x31; i++) {
        if (CModel[i].hobj != NULL) {
          NuHGobjDestroy(CModel[i].hobj);
          CModel[i].hobj = 0;
        }
  }
  return;
}
