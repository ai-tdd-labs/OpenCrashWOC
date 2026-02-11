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

extern int Chase;
extern int best_cRPos;
extern int plr_tocam_turn;
extern int plr_tocam_wait;

void UpdateChaseRunAnim(struct creature_s *plr) {
  s32 turn;
  s32 iVar4;
  s32 rot;
  
  if (((plr->obj).anim.newaction == 0x3a) &&
     ((((Chase[0].status == 2 || (Chase[1].status == 2)) || (Chase[2].status == 2))
      && (((best_cRPos != NULL) && (rot = RotDiff((plr->obj).hdg,best_cRPos->angle), rot = rot >= 0 ? rot : -rot,
      (rot < 0x2000))))))) {
        if ((plr_tocam_wait != 0) && (plr_tocam_wait = plr_tocam_wait + -1, plr_tocam_wait == 0)) {
          plr_tocam_wait = (qrand() * 0x3c >> 0x10) + 0x3c;
          if (plr_tocam_turn == -1) {
            iVar4 = qrand();
              if (iVar4 < 0) {
                iVar4 += 0x7fff;
              }
              plr_tocam_turn = iVar4 >> 0xf;
          }
          else {
            if (plr_tocam_turn == 1) {
            iVar4 = -qrand();
              if (iVar4 < 0) {
                iVar4 += 0x7fff;
              }
            plr_tocam_turn = iVar4 >> 0xf;
            } else {
              if (qrand() < 0x8000) {
                  turn = -1;
              } else {
                turn = 1;
              }
                plr_tocam_turn = turn;
            }
          }
        }
  }
  else {
    plr_tocam_turn = 0;
    plr_tocam_wait = (qrand() * 0x3c >> 0x10) + 0x3c;
  }
  if ((plr->obj).anim.newaction == 0x3a) {
    if (plr_tocam_turn < 0) {
      (plr->obj).anim.newaction = 0x32;
    }
    else if (plr_tocam_turn > 0) {
      (plr->obj).anim.newaction = 0x39;
    }
    else {
      (plr->obj).anim.newaction = 0x3a;
    }
  }
  return;
}
