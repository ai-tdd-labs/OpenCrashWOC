//#include "gamecode/game_obj.h"

/* TODO

float CreatureTopBelow(struct nuvec_s *pos, u32 obj_flags) {
    struct obj_s *obj;
    struct nuvec_s obj_pos;
    float top;
    float shadow;
    float dz;
    float dx;
    s32 i;
    
    shadow = 2000000.0f;
    for(i = 0; i < GAMEOBJECTCOUNT; i++) {
        obj = pObj[i];
        if (((obj != NULL) && (obj->invisible == 0)) && ((obj->flags & obj_flags) != 0)) {
            if ((obj->flags & 0x2000) != 0) {
                if (obj->draw_frame == 0) {
                    continue;
                }
                if ((obj->pLOCATOR != NULL) && (obj->model->pLOCATOR[0] != NULL)) {
                    obj_pos.x = obj->pLOCATOR->_30;
                    obj_pos.y = obj->pLOCATOR->_31;
                    obj_pos.z = obj->pLOCATOR->_32;
                } else {
                    goto here;
                }
            }
            else
            {
                here:
                obj_pos = obj->pos;
            }
            if ((obj->flags & 0x8000) != 0) {
                if (CylinderCuboidOverlapXZ(pos, 0.0f, obj, &obj_pos) == 0) {
                    continue;
                }
            }
            else {
                dx = pos->x - obj_pos.x;
                dz = pos->z - obj_pos.z;
                dx *= dx;
                dz *= dz;
                if ((dx + dz) > (obj->RADIUS * obj->RADIUS)) {
                    continue;
                }
            }
            
            top = (obj->top * obj->SCALE + obj->pos.y);
            if ((shadow == 2000000.0f) || (top > shadow)) {
                shadow = top;
            }
        }
    }
    return shadow;
}
