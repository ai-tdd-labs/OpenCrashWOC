void DrawWeatherBoss_a(BOSSSTRUCT *Boss) {
  struct nuvec_s Pos;
  
  Pos = (Boss->Position);
  NuMtxSetScale(&mTEMP,SetNuVecPntr(WBSCALE,WBSCALE,WBSCALE));
  NuMtxRotateY(&mTEMP,(Boss->AngleY * 182.04445f));
  NuMtxTranslate(&mTEMP,&Pos);
  if (Boss->Action == 5) {
      Boss->Seen = MyDrawModelNew(&Boss->BonesDraw,&mTEMP,Boss->Locator);
  } else {
       Boss->Seen = MyDrawModelNew(&Boss->MainDraw,&mTEMP,Boss->Locator);
  }
}
