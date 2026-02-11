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

extern int font3d_initialised;
extern int font3d_scene;
extern int font3d_scene2;
extern int superbuffer_end;
extern int superbuffer_ptr;

void LoadFont3D() {

  font3d_initialised = 0;
  font3d_scene2 = NuSceneReader2(&superbuffer_ptr,&superbuffer_end,"stuff\\font.nus");
  if (font3d_scene2 != NULL) {
    font3d_scene = font3d_scene2->gscene;
    InitFont3D(font3d_scene);
  }
  return;
}
