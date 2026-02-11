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

extern int faceonitem_cnt;
extern int faceonmtl_cnt;
extern int otitem_cnt;

static void NuMtlRenderUpd(void) {
  faceonitem_cnt = 0x200;
  faceonmtl_cnt = 0;
  otitem_cnt = 0x7e8;
  NuTexSetTexture(0,0);
  return;
}
