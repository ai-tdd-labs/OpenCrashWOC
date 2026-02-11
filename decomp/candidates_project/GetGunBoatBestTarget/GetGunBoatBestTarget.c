float GetGunBoatBestTarget(float Best,struct nuvec_s **TargetPos,struct nuvec_s **Vel,s32 *Moving) {
  struct nuvec_s CamDir;
  struct nuvec_s Rel;
  s32 i;
  float Mag;
  float Dot;
  
  NuVecMtxRotate(&CamDir,SetNuVecPntr(0.0f,0.0f,1.0f),&GameCam[0].m);
  for(i = 0; i < 4; i++) {
    if (GunBoatList[i].Active != 0) {
      if (GunBoatList[i].Action != 2) {
        NuVecSub(&Rel,&GunBoatList[i].Position,&GameCam[0].pos);
        Mag = NuVecMag(&Rel);
        if (Mag > 10.0f) {
          NuVecScale(&Rel,&Rel,(1.0f / Mag));
          Dot = DotProduct(&CamDir,&Rel);
          if (Dot > Best) {
            *TargetPos = &GunBoatList[i].Position;
            *Vel = &v000;
            *Moving = 0;
            Best = Dot;
          }
        }
      }
    }
  }
  return Best;
}
