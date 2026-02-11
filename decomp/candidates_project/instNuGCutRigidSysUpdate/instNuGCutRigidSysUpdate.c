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

extern int newvisibilitystate;
extern int s8;
extern int u8;

static void instNuGCutRigidSysUpdate(struct instNUGCUTSCENE_s* icutscene, float current_frame) {   
    struct NUGCUTRIGID_s *rigid; 
    struct instNUGCUTRIGID_s *irigid;
    s32 i;
    s32 j;
    struct numtx_s mtx;
    struct NUGCUTSCENE_s *cutscene;
    struct NUGCUTRIGIDSYS_s *rigidsys;
    struct instNUGCUTRIGIDSYS_s *irigidsys;
    s8 newvisibilitystate;
    struct NUGCUTLOCATORSYS_s *locatorsys;
    struct instNUGCUTLOCATORSYS_s *ilocatorsys;
    s32 locix;
    // ------------------------------------------
    struct NUGCUTLOCATOR_s *locator;
    struct instNUGCUTLOCATOR_s *iloctemp;

    
    cutscene = icutscene->cutscene;
    irigidsys = icutscene->irigids;
    rigidsys = cutscene->rigids;
    
    for(i = 0; i < rigidsys->nrigids; i++)
    {
        rigid = &rigidsys->rigids[i];
        irigid = &irigidsys->irigids[i];

        if ((rigid->flags & 2)) {
            continue;
        }
        
        if ((rigid->flags & 4) == 0) {
            continue;
        }
        
        if ((rigid->visibiltyanim != NULL) &&
        (StateAnimEvaluate(rigid->visibiltyanim,&irigid->visibleframeix, (u8 *)&newvisibilitystate,current_frame) != 0)) {
            if (newvisibilitystate != 0) {
                ((irigid->special).special)->instance->flags.visible = 1;
            }
            else {
                ((irigid->special).special)->instance->flags.visible = 0;
            }
        }
        
        if ( ((irigid->special).special)->instance->flags.visible != 0)
        {
            NuGCutRigidCalcMtx(rigid, current_frame, &mtx);
            
            if ((icutscene->has_mtx) != 0)
            {
                NuMtxMul(&mtx, &mtx, &icutscene->mtx);
            }
            
            (irigid->special.special)->instance->mtx = mtx;

            if (((((irigid->special).special)->instance->flags.visible) != 0) && (rigid->first_locator != 0xff)) {
                locatorsys = cutscene->locators;
                ilocatorsys = icutscene->ilocators;
                
                for (j = 0; j < rigid->nlocators; j++) {
                    locator = &locatorsys->locators[rigid->first_locator + j];
                    iloctemp = &ilocatorsys->ilocators[rigid->first_locator + j];
                    instNuGCutLocatorUpdate(icutscene, locatorsys,  iloctemp, locator, current_frame, &mtx);
                }
            }
        }
    }
    return;
}
