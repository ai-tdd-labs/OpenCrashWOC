/*

s32 NearestSplinePoint(struct nuvec_s *pos,struct nugspline_s *spl) {
  s32 index;
  s32 i;
  s32 d;
  s32 d0;
  
  index = -1;
  if ((spl != NULL)) {
    for (i = 0; i < spl->len; i++) {
      d = NuVecDistSqr(pos,(spl->pts + i * spl->ptsize),NULL);
      if ((index == -1) || (d < d0)) {
        index = i;
        d0 = d;
      }
    }
  }
  return index;
}
