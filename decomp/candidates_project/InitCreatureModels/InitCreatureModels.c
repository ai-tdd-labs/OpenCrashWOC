#include "../nu.h"
#include "gamecode/main.h"
/*

void InitCreatureModels(void) {
  s32 i;

  for (i = 0; i < 9; i++) {
    Character[i].used = '\0';
    Character[i].on = '\0';
    Character[i].off_wait = '\0';
  }
  GAMEOBJECTCOUNT = 0;
  InitSkinning(0);
  LoadCharacterModels();
  CloseSkinning();
  return;
}
