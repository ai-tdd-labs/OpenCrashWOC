#include "../nu.h"
#include "gamecode/main.h"
/*

void InitCreatures(void) {
  s32 i;

  InitAI();
  InitChases();
  ResetChases();
  i = LDATA->flags & 1;
  if ((LDATA->flags & 1) != 0) {
    AddCreature((s32)LDATA->character,0,-1);
    i = 1;
  }
  PLAYERCOUNT = i;
  return;
}
