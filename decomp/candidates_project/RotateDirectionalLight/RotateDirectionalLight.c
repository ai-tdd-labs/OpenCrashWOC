void RotateDirectionalLight(struct nuvec_s *v,s32 xrot,s32 yrot) {
  NuVecRotateX(v,&v001,xrot);
  NuVecRotateY(v,v,yrot);
  return;
}
