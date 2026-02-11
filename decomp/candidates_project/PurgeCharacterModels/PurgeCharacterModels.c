#include "gamecode/creature.h"

void PurgeCharacterModels(void) {
  s32 i;

  for (i = 0; i < 0xbf; i++) {
    CRemap[i] = -1;
    CLetter[i] = '?';
  }
    i = 48;
  while (i >= 0) {
    CModel[i].hobj = NULL;
    i--;
  }
  return;
}
