//NGC MATCH

void DestroyAllNitroCrates(CrateCubeGroup *group,CrateCube *crate) {
  CrateCubeGroup *group2;
  CrateCube *crate2;
  struct nuvec_s pos;
  s32 i;
  s32 j;
  
  group2 = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group2++) {
      crate2 = Crate + group2->iCrate;
      for(j = 0; j < group2->nCrates; j++, crate2++) {
          if ((crate2->on != 0) && (GetCrateType(crate2,0) == 0x10)) {
            CrateOff(group2,crate2,0,0);
          }
      }
  }
  pos.x = (crate->pos).x;
  pos.y = (crate->pos).y + 0.25f;
  pos.z = (crate->pos).z;
  temp_pGroup = group;
  temp_pCrate = crate;
  AddKaboom(0x20,&pos,0.0f);
  crate->newtype = 0xf;
  crate->metal_count = 1;
  JudderGameCamera(GameCam,0.5f,NULL);
  GameSfx(0x33,&crate->pos);
  return;
}
