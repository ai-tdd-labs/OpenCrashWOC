#include "../nu.h"
#include "gamecode/main.h"
/*

void DrawFade(void) {
  if ((Cursor.menu != '\x02') && (fadeval != 0)) {
    NuRndrRect2di(0,0,SWIDTH << 4,SHEIGHT << 3,fadecol,fade_mtl);
  }
  return;
}
