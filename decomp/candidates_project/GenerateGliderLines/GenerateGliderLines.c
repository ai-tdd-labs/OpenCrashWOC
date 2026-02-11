static void GenerateGliderLines(GLIDERSTRUCT *Glider,struct nuvec_s *Lines) {
  struct numtx_s Mat;
  s32 i;
  
  NuMtxSetRotationZ(&Mat,(Glider->TiltZ * 182.04445f));
  NuMtxRotateX(&Mat,((Glider->TiltX + 15.0f) * 182.04445f));
  NuMtxRotateY(&Mat,(Glider->AngleY * 182.04445f));
  NuMtxTranslate(&Mat,&Glider->Position);
  for(i = 0; i < 5; i++) {
    NuVecMtxTransform(&Lines[i],&GliderCollPoints[i],&Mat);
  }
  Lines[5] = *Lines;
}
