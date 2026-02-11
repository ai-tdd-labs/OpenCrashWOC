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

void InitAI(void) {
  s32 i;
  
  LEVELAITYPES = 0;
  for(i = 0; i < 0x6b; i++) {
    if (CRemap[AIType[i].character] != -1) {
      LevelAIType[LEVELAITYPES++] = i;
    }
  }
  LEVELAICOUNT = 0;
  if (Level != 0x28) {
    LoadAI();
  }
  ResetAI();
  return;
}
