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

extern int AshesMechInPos;
extern int CData;
extern int CModel;
extern int CRemap;
extern int Level;
extern int ShadNorm;
extern int VEHICLECONTROL;
extern int VTog;
extern int best_cRPos;
extern int mTEMP;
extern int pVTog;
extern int player;
extern int temp_xrot;
extern int temp_zrot;
extern int vtog_action;
extern int vtog_angle;
extern int vtog_blend;
extern int vtog_duration;
extern int vtog_time;

void DrawNextVehicle(s32 render) {
  struct vtog_s *tog;
  struct CharacterModel* model;
  struct nuvec_s pos;
  s32 i;
  s32 vehicle;
  struct anim_s anim;
  float y;
  
  vehicle = (s32)(player->obj).vehicle;
  if (vehicle != -1) {
      i = CRemap[vehicle];
        if(i != -1) {
                model = &CModel[i];
                if ((Level == 0x16) && ((VEHICLECONTROL == 0 || (vtog_time < vtog_duration)))) {
                  pos = AshesMechInPos;
                  y = NewShadow(&pos,0.0f);
                  if (y != 2000000.0f) {
                    pos.y = y;
                    FindAnglesZX(&ShadNorm);
                  }
                  else {
                    temp_xrot = temp_zrot = 0;
                  }
                  NuMtxSetRotationY(&mTEMP,vtog_angle + 0x8000);
                  NuMtxRotateZ(&mTEMP,temp_zrot);
                  NuMtxRotateX(&mTEMP,temp_xrot);
                  NuMtxTranslate(&mTEMP,&pos);
                  ResetAnimPacket(&anim,0x62);
                  DrawCharacterModel(model,&anim,&mTEMP,NULL,render,NULL,NULL,NULL,NULL);
                }
                tog = VTog;
                for(i = 0; i < 6; i++) {
                  if (((Level != 0xf) || (i < 2)) &&
                     (((vtog_time < vtog_duration && ((vtog_blend != 0 && (tog == pVTog)))) ||
                      ((best_cRPos != NULL && (((VEHICLECONTROL != 1 && ((tog)->pTRIGGER != NULL)) &&
                        (FurtherALONG((s32)(tog)->iRAIL,(s32)(tog)->iALONG,(tog)->fALONG,(s32)best_cRPos->iRAIL,
                                              (s32)best_cRPos->iALONG,best_cRPos->fALONG) != 0)))))))) {
                    if ((tog)->type != '\0') {
                      return;
                    }
                    NuMtxSetIdentity(&mTEMP);
                    NuMtxRotateY(&mTEMP,(tog)->yrot + 0x8000);
                    NuMtxRotateZ(&mTEMP,(uint)(tog)->zrot);
                    NuMtxRotateX(&mTEMP,(uint)(tog)->xrot);
                    pos.x = ((tog)->pos).x;
                    pos.y = ((tog)->pos).y - CData[vehicle].min.y * CData[vehicle].scale;
                    pos.z = ((tog)->pos).z;
                    NuMtxTranslate(&mTEMP,&(tog)->pos);
                    ResetAnimPacket(&(tog)->anim,vtog_action);
                    DrawCharacterModel(model,&(tog)->anim,&mTEMP,NULL,render,NULL,NULL,NULL,NULL);
                    return;
                  }
                  tog++;
                }
        }
  }
  return;
}
