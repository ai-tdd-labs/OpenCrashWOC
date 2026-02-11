#include "../nu.h"
#include "gamecode/main.h"
/*

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
