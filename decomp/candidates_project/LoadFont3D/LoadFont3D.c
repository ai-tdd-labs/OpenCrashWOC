#include "../nu.h"
#include "gamecode/main.h"
/*

void LoadFont3D() {

  font3d_initialised = 0;
  font3d_scene2 = NuSceneReader2(&superbuffer_ptr,&superbuffer_end,"stuff\\font.nus");
  if (font3d_scene2 != NULL) {
    font3d_scene = font3d_scene2->gscene;
    InitFont3D(font3d_scene);
  }
  return;
}
