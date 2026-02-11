#include "gamecode/camera.h"

/*

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
