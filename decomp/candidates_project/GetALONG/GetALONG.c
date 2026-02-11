extern int TempRPos;
extern int nRAILS;
extern int temp_iALONG;
extern int temp_iRAIL;

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

void GetALONG(struct nuvec_s *pos,struct RPos_s *rpos,s32 iRAIL,s32 iALONG,s32 info) {
  if (rpos == NULL) {
    rpos = &TempRPos;
  }
  if (nRAILS != 0) {
    ComplexRailPosition(pos,iRAIL,iALONG,rpos,0);
  }
  else {
    temp_iALONG = -1;
    temp_iRAIL = -1;
  }
  return;
}
