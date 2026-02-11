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

s32 FindAIType(char *name,s32 points) {
  s32 i;
  
  for (i = 0; i < 0x6b; i++) {
      if ((strcmp(name,AIType[i].name) == 0 && (points == AIType[i].points)) && (points < 9)) {
          return i;
      }
  }
  return -1;
}
