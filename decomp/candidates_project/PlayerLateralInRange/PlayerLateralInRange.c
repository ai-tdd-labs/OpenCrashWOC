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

s32 PlayerLateralInRange(float lateral,struct nuvec_s *pos,struct nuvec_s *p0,struct nuvec_s *p1,float distance) {
  if(NuFabs(RatioDifferenceAlongLine(lateral,RatioAlongLine(pos,p0,p1),p0,p1)) < distance) {
      return 1;
  }
  return 0;
}
