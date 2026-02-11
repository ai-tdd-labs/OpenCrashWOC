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

void UpdateDRAINDAMAGE(void) {
  struct creature_s *crunch;
  u16 xrot;
  u16 yrot;
  struct nuvec_s pos;
  struct nuvec_s v;
  s32 i;
  
  FindNearestCreature(&(player->obj).pos,0x7f,NULL);
  if (temp_creature_i != -1) {
    crunch = &Character[temp_creature_i];
  }
  else {
    crunch = NULL;
  }
  drain_wawa_model = NULL;
  i = (s32)CRemap[87];
  if (i != -1) {
    drain_wawa_model = &CModel[i];
    if ((drain_wawa_model->anmdata[0xc] != NULL) && (drain_wawa_model->anmdata[0xd] != NULL)) {
      drain_ang[0] = drain_ang[0] + 0x6bd;
      drain_ang[1] = drain_ang[1] + 0x492;
      drain_ang[2] = drain_ang[2] + 0x8e8;
      if (crunch == NULL) {
        if (drain_wawa_ok == 0) {
            goto jump;
        }
      }
      else {
        pos.x = crunch->obj.pos.x - (NuTrigTable[crunch->obj.hdg] * 3.0f);
        pos.y = crunch->obj.pos.y + DRAINWAWADY;
        pos.z = crunch->obj.pos.z - (NuTrigTable[(crunch->obj.hdg + 0x4000) & 0xFFFF] * 3.0f);
      }
      NuVecSub(&v,&GameCam[0].pos,&drain_wawa_pos);
      xrot = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
      yrot = NuAtan2D(v.x,v.z);
      if (drain_wawa_ok == 0) {
        drain_wawa_ok = 1;
        drain_wawa_pos = pos;
        drain_wawa_xrot = xrot;
        drain_wawa_yrot = yrot;
        ResetAnimPacket(&drain_wawa_anim,0xc);
        GameSfx(0x1e,&drain_wawa_pos);
      }
      else {
        drain_wawa_xrot = SeekRot(drain_wawa_xrot,xrot,3);
        drain_wawa_yrot = SeekRot(drain_wawa_yrot,yrot,3);
      }
      drain_wawa_anim.oldaction = drain_wawa_anim.action;
      UpdateAnimPacket(CModel + i,&drain_wawa_anim,0.5f,0.0f);
      if ((drain_wawa_anim.flags & 1) == 0) {
          return;
      }
        if (drain_wawa_anim.action == 0xc) {
          if ((crunch == NULL) || (NuVecDistSqr(&drain_wawa_pos,&crunch->obj.pos,NULL) > 25.0f)) {
              drain_wawa_anim.newaction = 0xd;
              GameSfx(0x21,&drain_wawa_pos);
          }
          return;
        }
        drain_wawa_ok = 0;
        return;
jump:
        drain_wawa_ok = 0;
        return;
    }
      drain_wawa_model = NULL;
  }
  return;
}
