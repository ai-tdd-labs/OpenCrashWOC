void DrawAtlas(struct creature_s *Cre) {
  struct ATLASSTRUCT *Atlas;
  struct numtx_s Matrix;
  s32 i;
  
  Atlas = (struct ATLASSTRUCT *)Cre->Buggy;
  NuQuatToMtx(&Atlas->Quat,&Matrix);
  NuMtxTranslate(&Matrix,&Atlas->Position);
  i = CRemap[83];
  if (i != -1) {
    NuHGobjRndr(CModel[i].hobj,&Matrix,1,NULL);
  }
  return;
}
