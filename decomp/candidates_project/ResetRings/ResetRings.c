//NGC MATCH

void ResetRings(void) {
  struct nuvec_s *pos;
  struct nuvec_s v;
  s32 i;
  s32 temp;
  
  RINGCOUNT = 0;
  if (((SplTab[64].spl != NULL) && (ObjTab[103].obj.special != NULL)) && (ObjTab[104].obj.special != NULL)) {
    RINGCOUNT = (s32)(SplTab[64].spl)->len / 2;
    if (0x30 < RINGCOUNT) {
      RINGCOUNT = 0x30;
    }
    pos = (struct nuvec_s *)(SplTab[64].spl)->pts;
    GetALONG(pos,NULL,-1,-1,1);
    if (AheadOfCheckpoint(temp_iRAIL,temp_iALONG,temp_fALONG) != 0) {
      i_ring = 0;
    }
    for (i = 0; i < RINGCOUNT; i++, pos+=2) {
        NuVecSub(&v,(pos + 1),pos);
        ring_yrot[i] = (short) NuAtan2D(v.x,v.z);
        temp = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
        ring_xrot[i] = -(short)temp;
        if (i == i_ring) {
          ring_pos = *pos;
        }
    }
  }
  return;
}
