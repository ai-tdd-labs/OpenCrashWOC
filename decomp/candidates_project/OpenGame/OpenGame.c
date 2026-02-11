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

extern int Game;
extern int HData;

void OpenGame(void) {
  s32 i;
  s32 j;
  s32 k;
  
  for (k = 0; k < 0x23; k++) {
    Game.level[k].flags = 0;
  }
  for (k = 0; k < 6; k++) {
    Game.hub[k].flags = 0xff;
    Game.hub[k].crystals = '\0';
    if (k < 5) {
      for (i = 0, j = 0; i < 6; i++, j++) {
        if (HData[k].level[i] != -1) {
          if (j < 5) {
            Game.level[HData[k].level[i]].flags = 0x1f;
            Game.hub[k].crystals = Game.hub[k].crystals + '\x01';
          }
          else {
            Game.level[HData[k].level[i]].flags = 0x800;
          }
        }
      }
    }
    else {
      for (i = 0; i < 5; i++) {
        if (HData[k].level[i] != -1) {
          Game.level[HData[k].level[i]].flags = 0x17;
        }
      }
    }
  }
  Game.level[1].flags = Game.level[1].flags | 0x40;
  Game.level[17].flags = Game.level[17].flags | 0x80;
  Game.level[7].flags = Game.level[7].flags | 0x100;
  Game.level[19].flags = Game.level[19].flags | 0x200;
  Game.level[10].flags = Game.level[10].flags | 0x400;
  Game.level[4].flags = Game.level[4].flags | 0x20;
  Game.level[12].flags = Game.level[12].flags | 0x20;
  Game.level[14].flags = Game.level[14].flags | 0x20;
  Game.level[5].flags = Game.level[5].flags | 0x20;
  Game.level[20].flags = Game.level[20].flags | 0x20;
  Game.level[33].flags = Game.level[33].flags | 0x20;
  Game.level[27].flags = Game.level[27].flags | 0x20;
  Game.level[28].flags = Game.level[28].flags | 0x20;
  Game.level[29].flags = Game.level[29].flags | 0x20;
  Game.level[32].flags = Game.level[32].flags | 0x20;
  Game.level[9].flags = Game.level[9].flags | 0x20;
  Game.powerbits = 0xff;
  Game.cutbits = (u32)-1;
  CalculateGamePercentage(&Game);
  ResetBonus();
  ResetDeath();
  ResetGemPath();
  return;
}
