//#include "gamecode/game_obj.h"

/* TODO

s32 HitItems(struct obj_s *obj) {
  struct obj_s *cyl;
  s32 i;
  
  if (level_part_2 != 0) {
    return 0;
  }
  for(i = 0; i < 64; i++) {
      cyl = pObj[i];
      if ((((cyl != NULL) && (cyl->dead == 0)) && (cyl->invisible == 0)) &&
         (((cyl->flags & 0x10) != 0 && (GameObjectOverlap(obj,cyl,0) != 0)))) {
        PickupItem(cyl);
        return 1;
      }
  }
  return 0;
}
