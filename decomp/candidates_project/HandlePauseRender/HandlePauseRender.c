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

extern int Paused;
extern int pause_rndr_mtl;
extern int pause_rndr_on;

void HandlePauseRender(int pause_count) {
  static s32 old_pause_state;
  s32 vp_x;
  s32 vp_y;
  struct nuviewport_s *vp;

  vp_x = (s32)((float)pause_count * 27.0f / 30.0f);
  vp_y = (s32)((float)pause_count * 19.0f / 30.0f);
  if ((pause_rndr_on != 0) && (NuRndrBeginScene(1) != 0)) {
    vp = NuVpGetCurrentViewport();
    NuRndrClear(0xb,0,1.0f);
    NuRndrRectUV2di(vp_x,vp_y,vp->width - vp_x,vp->height - vp_y,0.0f,0.0f,1.0f,1.0f,-1,pause_rndr_mtl);
    NuRndrEndScene();
  }
  if (Paused != 0) {
    if (old_pause_state == 0) {
      NudxFw_MakeBackBufferCopy(1);
      GS_CopyFBToPause();
      pause_rndr_on = 1;
    }
    if (Paused != 0) {
        goto block_end;
    }
  }
  if (old_pause_state != 0) {
    pause_rndr_on = 0;
  }
block_end:
  old_pause_state = Paused;
  return;
}
