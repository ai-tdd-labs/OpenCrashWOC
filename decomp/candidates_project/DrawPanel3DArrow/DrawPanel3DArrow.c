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
extern int ObjTab;
extern int arrow_xrot;
extern int arrow_yrot;

void DrawPanel3DArrow(void) {
  if (ObjTab[108].obj.special != NULL) {
    DrawPanel3DObject(0x6c,0.0f,0.5f,1.0f,0.125f,0.125f,0.125f,arrow_xrot,RotDiff(GameCam[0].yrot,arrow_yrot),0,ObjTab[108].obj.scene,
                      ObjTab[108].obj.special,0);
  }
  return;
}
