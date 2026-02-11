//NGC MATCH

s32 PlayerLateralInRange(float lateral,struct nuvec_s *pos,struct nuvec_s *p0,struct nuvec_s *p1,float distance) {
  if(NuFabs(RatioDifferenceAlongLine(lateral,RatioAlongLine(pos,p0,p1),p0,p1)) < distance) {
      return 1;
  }
  return 0;
}
