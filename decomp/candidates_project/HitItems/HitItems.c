typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef unsigned int uint;
typedef int bool32;
#ifndef NULL
#define NULL ((void*)0)
#endif

struct nuvec_s { f32 x; f32 y; f32 z; };
struct nuvec4_s { f32 x; f32 y; f32 z; f32 w; };
struct nuinstance_s;
struct nugscn_s;
struct nugobj_s;
struct creature_s;
struct creatcmd_s;
struct numtx_s;
struct nuwindgrp_s;

extern int level_part_2;
extern int pObj;

s32 HitItems(struct obj_s *obj) {
  struct obj_s *cyl;
  s32 i;
  
  if (level_part_2 != 0) {
    return 0;
  }
  for(i = 0; i < 64; i++) {
      cyl = pObj[i];
      if ((((cyl != NULL) && (cyl->dead == 0)) && (cyl->invisible == 0)) &&
         (((cyl->flags & 0x10) != 0 && (GameObjectOverlap(obj,cyl,0) != 0)))) {
        PickupItem(cyl);
        return 1;
      }
  }
  return 0;
}
