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
