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
