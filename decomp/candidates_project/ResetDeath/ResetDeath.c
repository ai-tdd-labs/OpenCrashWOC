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

extern int Death;
extern int Hub;
extern int Level;
extern int Rail;
extern int death_begun;
extern int death_obj;
extern int last_level;

void ResetDeath(void) {
  struct rail_s* rail;

  death_begun = 0;
  Death = 0;
  rail = &Rail[6];

  if ((((rail->type != -1) && (Level == 0x25)) && (Hub == 5)) &&
     ((last_level != 0x26 && (last_level != -1)))) {
    StartTransporter(&death_obj,(struct nuvec_s *)(&(rail->pINPLAT)->pts[((rail->pINPLAT)->len + -1) * (s32)(rail->pINPLAT)->ptsize]));
    Death = 2;
    death_begun = 1;
  }
  return;
}
