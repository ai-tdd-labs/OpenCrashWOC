//NGC MATCH

s32 HitCrates(struct obj_s *obj,s32 destroy) {
  if ((level_part_2 == 0) &&
     (InCrate(obj->pos.x,obj->pos.z,(obj->top * obj->SCALE + obj->pos.y),(obj->bot * obj->SCALE + obj->pos.y),obj->RADIUS) != 0)) {
        if ((destroy == 1) || ((((destroy == 2 && (temp_crate_type != 7)) && (temp_crate_type != 0xe)) && (temp_crate_type != 0x11)))) {
          BreakCrate(temp_pGroup,temp_pCrate,temp_crate_type,(uint)obj->attack);
        }
        return 1;
  }
  else {
    return 0;
  }
}
