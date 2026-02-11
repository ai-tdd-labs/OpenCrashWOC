//NGC MATCH

void InitBugAreas(void) {
  s32 index;
  s32 i;
  struct nuvec_s *vec;
  
  for(i = 0; i < 4; i++) {
    BugArea[i].in_iRAIL = -1;
    BugArea[i].out_iRAIL = -1;
  }
  if ((SplTab[67].spl != NULL)) {
    for(i = 0; i < (SplTab[67].spl)->len; i++) {
      vec = (struct nuvec_s *)(SplTab[67].spl->pts + (i * (s32)(SplTab[67].spl)->ptsize));
      GetALONG(vec,NULL,-1,-1,1);
      if ((temp_iRAIL != -1) && (Rail[temp_iRAIL].type == 0)) {
        if ((i & 1) != 0) {
          index = ((i + ((u32)i >> 0x1f)) & 0xFFFFFFFE) >> 1;
          BugArea[index].out_iRAIL = temp_iRAIL;
          BugArea[index].out_iALONG = temp_iALONG;
          BugArea[index].out_fALONG = temp_fALONG;
        }
        else {
          index = ((i + ((u32)i >> 0x1f)) & 0xFFFFFFFE) >> 1;
          BugArea[index].in_iRAIL = temp_iRAIL;
          BugArea[index].in_iALONG = temp_iALONG;
          BugArea[index].in_fALONG = temp_fALONG;
        }
      }
    }
  }
  bug_splratio = 0.0f;
  return;
}
