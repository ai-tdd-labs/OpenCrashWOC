typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned int uint;
typedef int bool32;

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
