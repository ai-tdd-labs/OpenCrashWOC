#include "gamecode/creature.h"

void NewRumble(struct rumble_s *rumble,s32 power) {
  if ((rumble->frame != 0) && (power <= (rumble->power * rumble->frame) / rumble->frames)) {
    return;
  }
  rumble->power = (u8)power;
  rumble->frames = (power * 0x3c) >> 8;
  rumble->frame = (power * 0x3c) >> 8;
  return;
}
