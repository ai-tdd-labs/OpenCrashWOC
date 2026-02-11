//#include "gamecode/game_obj.h"

/* TODO

void ResetProjectiles(void) {
  s32 i;
  struct projectile_s *p;
  
  p = Projectile;
  for(i = 0; i < 0x10; i++) {
    if (p->active != 0) {
      RemoveGameObject(&p->obj);
      p->active = 0;
    }
    p++;
  }
  return;
}
