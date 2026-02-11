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

extern int CModel;
extern int CRemap;
extern int ChrisJointList;
extern int ChrisJointOveride;
extern int ChrisNumJoints;
extern int GameTimer;
extern int LBIT;
extern int NUSTENCIL_NOSTENCIL;
extern int NUSTENCIL_REPLACE_NODRAW;
extern int Paused;
extern int VEHICLECONTROL;
extern int glass_draw;
extern int jeep_draw;
extern int player;
extern int plr_render;
extern int tmtx;
extern int u8;

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

s32 DrawCharacterModel(struct CharacterModel* model,struct anim_s* anim,struct numtx_s* mC,struct numtx_s* mS,
    s32 render,struct numtx_s* mR,struct numtx_s* loc_mtx,struct nuvec_s* loc_mom,struct obj_s* obj) {

    float** dwa;
    s32 action;
    float time;
    struct CharacterModel* model2;
    s32 Drawn = 0;
    struct NUJOINTANIM_s joint[4];
    struct NUJOINTANIM_s* pJ;
    s32 nJ;
    s32 i;
    short layertab[2] = { 0, 1 };
    short* layer;
    s32 nlayers;

    if (jeep_draw != 0) {
        for (i = 0; i < 4; i++) {
            joint[i].rx = ((GameTimer.frame % 0x3c) * 0x10000) / 0x3c * 9.58738e-05f;
            joint[i].ry = 0;
            joint[i].rz = 0;
            joint[i].tx = joint[i].ty = joint[i].tz = 0;
            joint[i].sx = joint[i].sy = joint[i].sz = 1;
            joint[i].joint_id = i;
        }
    } else {
        joint->rx = (f32)((u16)-(player->obj).target_xrot) * 9.58738e-05f;
        joint->ry = (f32)((u16)-(player->obj).target_yrot) * 9.58738e-05f;
        joint->rz = 0.0f;
        joint->tx = joint->ty = joint->tz = 0.0f;
        joint->sx = joint->sy = joint->sz = 1.0f;
        joint->joint_id = (u8)jointnum;
        joint->flags = 1;
    }
    if (mC == NULL) {
        Drawn = 0;
        goto Exit;
    }
    nlayers = 1;
    if (model->character == 0) {
        nlayers = 2;
    }
    if (anim != NULL) {
        if ((anim->blend != 0)
            && ((((u16)anim->blend_src_action <= 0x75) && (model->fanmdata[anim->blend_src_action] != NULL))
                && (((u16)anim->blend_dst_action <= 0x75) && (model->fanmdata[anim->blend_dst_action] != NULL))))
        {
            dwa = NuHGobjEvalDwaBlend(
                nlayers, (short*)&layertab[0], model->fanmdata[anim->blend_src_action], anim->blend_src_time,
                model->fanmdata[anim->blend_dst_action], anim->blend_dst_time,
                (float)anim->blend_frame / (float)anim->blend_frames
            );
        } else if (
            (anim->blend == 0) &&
            ((((u16)anim->action <= 0x75) && (model->fanmdata[anim->action] != NULL)))) {
            dwa = NuHGobjEvalDwa(1, NULL, model->fanmdata[anim->action], anim->anim_time);
        } else {
            dwa = NULL;
        }
    } else {
        dwa = NULL;
    }
    
    model2 = model;
    if (model->character == 0x54) {
        if ((LBIT & 0x0000000400000040)) {
            if ((s32)CRemap[115] != -1) {
                model2 = &CModel[(s32)CRemap[115]];
            }
        } else if ((s32)CRemap[0] != -1) {
                model2 = &CModel[(s32)CRemap[0]];
        }
    } else if (model->character == 0x9f) {
        if (CRemap[8] != -1) {
            model2 = &CModel[CRemap[8]];
        }
    }
    pJ = NULL;
    nJ = 0;
    if ((((jeep_draw == 0) && (plr_render != 0))
         && ((player->target != 0 && ((VEHICLECONTROL != 1 || ((player->obj).vehicle == -1))))))
        && ((model2->character == 0 || ((model2->character == 0x54 || (model2->character == 0x8c))))))
    {
        pJ = joint;
        nJ = 1;
    }
    if (ChrisJointOveride != 0) {
        pJ = ChrisJointList;
        nJ = ChrisNumJoints;
    }
    if (anim != NULL) {
        if (anim->blend != 0) {
            if (((((u16)anim->blend_src_action < 0x76) && (model2->anmdata[anim->blend_src_action] != NULL))
                 && ((u16)anim->blend_dst_action < 0x76))
                && (model2->anmdata[anim->blend_dst_action] != NULL)) {
                NuHGobjEvalAnimBlend(
                    model2->hobj, model2->anmdata[anim->blend_src_action], (f32)anim->blend_src_time,
                    model2->anmdata[anim->blend_dst_action], (f32)anim->blend_dst_time,
                    (f32)anim->blend_frame / (f32)anim->blend_frames, nJ, pJ, tmtx);
                action = (s32)anim->blend_dst_action;
                time = anim->blend_dst_time;
                goto LAB_8001dcf8;
            } 
            if (anim->blend != 0) {
                 goto NoModelAnim;      
            }
        }
        if (((u16)anim->action > 0x75) || (model2->anmdata[anim->action] == NULL)) {
            goto NoModelAnim;
        }
        NuHGobjEvalAnim(model2->hobj, model2->anmdata[anim->action], anim->anim_time, nJ, pJ, tmtx);
        action = (s32)anim->action;
        time = anim->anim_time;
    } else {
NoModelAnim:
        NuHGobjEval(model2->hobj, nJ, pJ, tmtx);
        action = -1;
    }
LAB_8001dcf8:
    if (glass_draw == 0) {
        StoreLocatorMatrices(model2, mC, tmtx, loc_mtx, loc_mom);
    }
    if (((action != -1) && (loc_mtx != NULL)) && ((Paused == 0 && (glass_draw == 0)))) {
        AddAnimDebris(model, loc_mtx, action, (float)time, obj);
    }
    if (render != 0) {
        if (((plr_render != 0) && (model->character == 0)) && ((player->obj).dead == 0x12)) {
            nlayers = 1;
        }
        Drawn = NuHGobjRndrMtxDwa(model->hobj, mC, nlayers, (short*)&layertab, tmtx, dwa);
        if (((Drawn != 0) && (obj != NULL)) && (obj->character == 0xb1)) {
            DrawProbeFX(obj);
        }
        if (mR != NULL) {
            NuHGobjRndrMtxDwa(model->hobj, mR, nlayers, (short*)&layertab, tmtx, dwa);
        }
        if (mS != NULL) {
            NuMtlSetStencilRender(NUSTENCIL_REPLACE_NODRAW);
            NuHGobjRndrMtx(model->hobj, mS, nlayers, (short*)&layertab, tmtx);
            NuMtlSetStencilRender(NUSTENCIL_NOSTENCIL);
        }
    }
Exit:
    plr_render = 0;
    jeep_draw = 0;
    return Drawn;
}
