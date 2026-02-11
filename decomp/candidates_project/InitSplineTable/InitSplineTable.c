/*

void InitSplineTable(void) {
  s32 i;
  
  if (world_scene[0] != NULL) {
    for (i = 0; i < 0x49; i++) {
      SplTab[i].spl = NULL;
      if (((SplTab[i].levbits >> Level) & 1) != 0) {
        SplTab[i].spl = NuSplineFind(world_scene[0],SplTab[i].name);
        if (SplTab[i].spl != NULL) {
          if ((SplTab[i].min > 0) && (SplTab[i].spl->len < SplTab[i].min)) {
            SplTab[i].spl = NULL;
          }
          else {
            if ((SplTab[i].max > 0) && ((SplTab[i].max >= SplTab[i].min  && (SplTab[i].spl->len > SplTab[i].max)))) {
              SplTab[i].spl = 0;
            }
          }
        }
      }
    }
  }
  else {
    for (i = 0; i < 0x49; i++) {
      SplTab[i].spl = NULL;
    }
  }
  return;
}
