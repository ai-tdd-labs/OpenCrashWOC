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

void ResetCRUNCHTIME(void) {
  struct nuvec_s v;
  s32 i;
  
  jcrunch = 0;
  crunch_vulnerable = 0;
  crunchtime_attack_phase = 0;
  earth_attack_on = 0;
  earth_attack_wait = 0;
  water_attack_on = 0;
  water_attack_wait = 0;
  water_last_chute = -1;
  fire_attack_on = -1;
  fire_attack_wait = 0;
  for(i = 0; i < 9; i++) {
    weather_attack_on[i] = 0;
  }
  weather_attack_wait = 0;
  CrunchTime_Intro = 1;
  InitVehMasks();
  InitVehMask(0,3);
  v.y = 5.0f;
  v.x = 0.0f;
  v.z = 0.0f;
  SetNewMaskStuff(0,&crunchtime_arena_midpos,&v,0.0f,-540.0f,1.0f,0,0,1.0f,0.0f);
  return;
}
