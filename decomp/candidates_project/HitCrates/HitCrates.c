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
extern int temp_crate_type;
extern int temp_pCrate;
extern int temp_pGroup;

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

s32 HitCrates(struct obj_s *obj,s32 destroy) {
  if ((level_part_2 == 0) &&
     (InCrate(obj->pos.x,obj->pos.z,(obj->top * obj->SCALE + obj->pos.y),(obj->bot * obj->SCALE + obj->pos.y),obj->RADIUS) != 0)) {
        if ((destroy == 1) || ((((destroy == 2 && (temp_crate_type != 7)) && (temp_crate_type != 0xe)) && (temp_crate_type != 0x11)))) {
          BreakCrate(temp_pGroup,temp_pCrate,temp_crate_type,(uint)obj->attack);
        }
        return 1;
  }
  else {
    return 0;
  }
}
