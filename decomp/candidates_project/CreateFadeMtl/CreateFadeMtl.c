#include "../nu.h"
#include "gamecode/main.h"
/*

void CreateFadeMtl() {
  struct numtl_s *mtl;

  mtl = NuMtlCreate(1);
  fade_mtl = mtl;
  fade_mtl->attrib.zmode = 3;
  fade_mtl->attrib.filter = 1;
  fade_mtl->attrib.lighting = 2;
  fade_mtl->attrib.utc = 1;
  fade_mtl->attrib.vtc = 1;
  fade_mtl->attrib.colour = 1;
  fade_mtl->attrib.alpha = 3;
  NuMtlUpdate(mtl);
  return;
}
