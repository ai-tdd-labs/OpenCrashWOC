void DrawGunBoat(GUNBOATSTRUCT *GunBoat) {
  struct nuvec_s ScaleVec = {1.5f, 1.5f, 1.5f};
  
  NuMtxSetScale(&mTEMP,&ScaleVec);
  NuMtxRotateX(&mTEMP,(int)(GunBoat->FireAngleX * 182.04445f));
  NuMtxRotateY(&mTEMP,(int)((GunBoat->AngleY + GunBoat->FireAngleY) * 182.04445f));
  NuMtxTranslate(&mTEMP,&GunBoat->Position);
  GunBoat->Seen = MyDrawModelNew(&GunBoat->MainDraw,&mTEMP,NULL);
}
