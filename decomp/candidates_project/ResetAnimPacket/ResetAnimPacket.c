#include "gamecode/creature.h"

void ResetAnimPacket(struct anim_s *anim,s32 action) {
  if (anim == NULL) {
    return;
  }
  anim->newaction = (short)action;
  anim->oldaction = (short)action;
  anim->action = (short)action;
  anim->anim_time = 1.0f;
  anim->blend = '\0';
  anim->flags = '\0';
  return;
}
