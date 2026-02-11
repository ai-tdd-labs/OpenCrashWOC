#include "gamecode/creature.h"

void CloseCreatures(void) {
  s32 i;
  
  for(i = 0; i < 9; i++) {
        if (Character[i].used != 0) {
          RemoveCreature(&Character[i]);
        }
  }
  for(i = 0; i < 0x31; i++) {
        if (CModel[i].hobj != NULL) {
          NuHGobjDestroy(CModel[i].hobj);
          CModel[i].hobj = 0;
        }
  }
  return;
}
