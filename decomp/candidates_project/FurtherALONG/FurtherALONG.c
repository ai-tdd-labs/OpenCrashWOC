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

s32 FurtherALONG(s32 iRAIL0,s32 iALONG0,float fALONG0,s32 iRAIL1,s32 iALONG1,float fALONG1) {
    if (iRAIL0 == -1) {
        return 0;
    }
  if ((((iRAIL0 == -1) || (iRAIL1 == -1)) || (Rail[iRAIL0].type != Rail[iRAIL1].type)) || (iRAIL0 < iRAIL1)) {
            return 0;
  }
    if (iRAIL0 > iRAIL1) { 
        return 1; 
    } 
    if (iALONG0 < iALONG1) { 
        return 0; 
    } else if (iALONG0 <= iALONG1) {
        return (fALONG0 > fALONG1);
    } else if (iALONG0 <= iALONG1) {
        return (fALONG0 > fALONG1);
    } 
  return 1;
}
