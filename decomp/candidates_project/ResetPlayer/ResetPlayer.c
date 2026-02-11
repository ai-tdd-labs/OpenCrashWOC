extern int ATLASCAMHEIGHT;
extern int CData;
extern int Demo;
extern int GameMode;
extern int LBIT;
extern int LDATA;
extern int Level;
extern int LivesLost;
extern int PLAYERCOUNT;
extern int VEHICLECONTROL;
extern int best_cRPos;
extern int bonus_restart;
extern int boss_dead;
extern int check_duration;
extern int check_time;
extern int cpGOTO;
extern int cpPOS;
extern int cp_goto;
extern int cp_iALONG;
extern int cp_iRAIL;
extern int force_panel_crate_update;
extern int force_panel_items_update;
extern int force_panel_lives_update;
extern int force_panel_wumpa_update;
extern int player;
extern int plr_crates;
extern int plr_rebound;
extern int point_duration;
extern int point_time;
extern int v000;

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

void ResetPlayer(s32 set) {
    struct creature_s* c;
    s32 start;
    s32 water;
    float y;

    if (PLAYERCOUNT != 0) {
        if (set != 0) {
            if ((Level != 0x11) || (cp_iALONG < 0x6b)) {
                edobjResetAnimsToZero();
            }
            c = player;
            PlayerStartPos(player, &(player->obj).startpos);
            start = 0;
            if ((bonus_restart != 0)
                && ((((LDATA->vBONUS).x != 0.0f || ((LDATA->vBONUS).y != 0.0f)) || ((LDATA->vBONUS).z != 0.0f))))
            {
                (c->obj).pos = (LDATA->vBONUS);
            } else {
                if (cp_goto != -1) {
                    (c->obj).pos = cpGOTO;
                } else {
                    if (cp_iRAIL != -1) {
                        (c->obj).pos = cpPOS;
                    } else {
                        start = 1;
                        (c->obj).pos = (c->obj).startpos;
                    }
                }
            }
            if (bonus_restart != 0) {
                bonus_restart = 0;
            }
            if (((Level == 2) && (cp_iRAIL == 0)) && (cp_iALONG == 0x23)) {
                water = 2;
            } else if ((LBIT & 0x0000000400000040) || ((Level == 2 && start))) {
                water = 1;
            } else {
                water = 0;
            }
            ResetPlayerMoves(c);
            GetTopBot(c);
            NewTopBot(&c->obj);
            OldTopBot(&c->obj);
            y = NewShadowPlat(&(c->obj).pos, 0.0f);
            GetSurfaceInfo(&c->obj, 1, y);
            if (cp_goto != -1) {
                cp_goto = -1;
            } else {
                if (water == 2) {
                    (c->obj).pos.y -= CData[32].min.y * (c->obj).SCALE;
                } else if (water != 0) {
                    if (cp_iRAIL != -1) {
                        (c->obj).pos.y -= (c->obj).bot * (c->obj).SCALE;
                    }
                } else if ((Level != 0x1d) && (y != 2000000.0f)) {
                    (c->obj).pos.y = (y - ((c->obj).bot * (c->obj).SCALE));
                }
            }
            (c->obj).oldpos = (c->obj).pos;
            (c->obj).mom = v000;
            ComplexRailPosition(&(c->obj).pos, (s32)(c->obj).RPos.iRAIL, (s32)(c->obj).RPos.iALONG, &(c->obj).RPos, 1);
            if ((VEHICLECONTROL == 2) || ((VEHICLECONTROL == 1 && ((c->obj).vehicle == 0x20)))) {
                (c->obj).thdg = (c->obj).hdg = 0;
            } else if (Level == 0x19) {
                (c->obj).hdg = 0x4000;
            } else if (best_cRPos != NULL) {
                (c->obj).hdg = (c->obj).thdg = best_cRPos->angle;
            }
            
            switch (c->obj.vehicle) {
                case 0x53:
                    ResetAtlas(c);
                    break;
                case 0x36:
                    if (Level == 0x18) {
                        SetWeatherStartPos(c);
                    }
                    break;
                case 99:
                    ResetJeep(c);
                    break;
            }
            
            plr_rebound = 0;
            ATLASCAMHEIGHT = 2.5f;
            ResetLights(&c->lights);
            
            if (((c->obj).mask != NULL) && ((c->obj).mask->active != 0)) {
                if ((c->obj).mask->active > 2) {
                    (c->obj).mask->active = 2;
                }
                ResetLights(&((c->obj).mask)->lights);
            }
            
        }
        plr_crates.count = plr_crates.draw = 0;
        plr_crates.frame = 0;
        (player->obj).scale = 1.0f;
        (player->obj).dead = 0;
        (player->obj).finished = 0;
        if ((((player->obj).mask != NULL) && ((player->obj).mask->active == 0)) && (LivesLost > 4)) {
            NewMask((player->obj).mask, &(player->obj).pos);
        }
    }
    if ((Demo == 0) && (GameMode != 1)) {
        force_panel_wumpa_update = 60;
        force_panel_crate_update = 60;
        force_panel_lives_update = 60;
        force_panel_items_update = 60;
    }
    boss_dead = 0;
    check_duration = 0.0f;
    check_time = 0.0f;
    point_duration = 0.0f;
    point_time = 0.0f;
}
