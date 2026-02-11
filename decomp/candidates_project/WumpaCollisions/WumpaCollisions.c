//#include "gamecode/game_obj.h"

/* TODO

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
