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

extern int GDeb;
extern int TimeTrial;
extern int WInfo;
extern int Wumpa;
extern int level_part_2;

s32 WumpaCollisions(struct obj_s *obj) {
    struct wumpa_s *wumpa;
    struct winfo_s* info;
    float dx;
    float dz;
    float y;
    float r2;
    s32 i;
    s32 got;
    s32 attack;
    s32 key;
    
    if (TimeTrial == 0) {
        
        if (level_part_2 != 0) {
            return 0;
        }
        
        r2 = GameObjectRadius(obj) + 0.2f;
        r2 *= r2;
        attack = obj->attack & 2;
        wumpa = Wumpa;
    
        for(i = 0; i < 320; i++, wumpa++) {
            if (wumpa->active == 2) {
                info = &WInfo[i & 7];
                y = wumpa->pos.y + info->dy;
                if ((obj->bot * obj->SCALE + obj->pos.y) < (y + 0.1f) && (obj->top * obj->SCALE + obj->pos.y) > (y - 0.1f)) {
                    dx = obj->pos.x - wumpa->pos.x;
                    dz = obj->pos.z - wumpa->pos.z;
                    if ((dx * dx + dz * dz) < r2) {
                        if ((attack != 0)) {
                            FlyWumpa(wumpa);
                            break; 
                        }
                        if (obj->dead == 0) {
                            AddScreenWumpa(wumpa->pos.x, y, wumpa->pos.z, 1);
                            key = -1;
                            AddFiniteShotDebrisEffect(&key, GDeb[130].i, &wumpa->pos, 1);
                            wumpa->active = 0;
                            GameSfx(0x2c, &obj->pos);
                        }
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
