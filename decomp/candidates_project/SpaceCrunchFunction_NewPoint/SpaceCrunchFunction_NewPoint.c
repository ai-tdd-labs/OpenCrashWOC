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

static s32 SpaceCrunchFunction_NewPoint(struct creature_s *c,struct nuvec_s *pos) {
  s32 i_old;
  s32 iVar4;

  i_old = c->ai.i0;
  switch(crunchtime_attack_phase) {
      case 0:
          //c->ai.i0 --> 0x1ED
        c->ai.i0 = crunchtime_phase1tab[1 - (u32)c->ai.count];
      break;
      case 1:
        c->ai.i0 = crunchtime_phase2tab[2 - (u32)c->ai.count];
      break;
      case 2:
        c->ai.i0 = crunchtime_phase3tab[4 - (u32)c->ai.count];
      break;
      case 3:
        c->ai.i0 = crunchtime_phase4tab[8 - (u32)c->ai.count];
      break;
      default:
        do {
          c->ai.i0 = (char)(qrand() / 0x4000) + 1;
        } while (c->ai.i0 == i_old);
      break;
  }
  c->ai.newpos = pos[c->ai.i0];
  if ((i_old == 0) || (c->ai.i0 == i_old)) {
    c->obj.anim.newaction = 0x2c;
  }
  else if (c->ai.i0 > i_old) {
    c->obj.anim.newaction = 0x32;
  } else {
    c->obj.anim.newaction = 0x39;
  }
  return 1;
}
