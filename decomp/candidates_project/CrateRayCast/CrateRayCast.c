//NGC MATCH

s32 CrateRayCast(struct nuvec_s *p0,struct nuvec_s *p1) {
  struct nuvec_s vMIN;
  struct nuvec_s vMAX;
  struct nuvec_s v0;
  struct nuvec_s v1;
  struct nuvec_s min;
  struct nuvec_s max;
  s32 i;
  s32 j;
  s32 face;
  s32 type;
  float ratio;
  CrateCubeGroup *group;
  CrateCube *crate;
  
  ratio = 1.0f;
  vMIN.x = (p0->x < p1->x) ? p0->x : p1->x;
  vMIN.z = (p0->z < p1->z) ? p0->z : p1->z;
  vMAX.x = (p0->x > p1->x) ? p0->x : p1->x;
  vMAX.z = (p0->z > p1->z) ? p0->z : p1->z;
  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      if ((((vMAX.x >= group->minclip.x) && (vMIN.x <= group->maxclip.x)) &&
          (vMAX.z >= group->minclip.z )) && (vMIN.z <= group->maxclip.z)) {
        v0.x = p0->x - (group->origin).x;
        v0.y = p0->y;
        v0.z = p0->z - (group->origin).z;
        NuVecRotateY(&v0,&v0,-(uint)group->angle);
        v1.x = p1->x - (group->origin).x;
        v1.y = p1->y;
        v1.z = p1->z - (group->origin).z;
        NuVecRotateY(&v1,&v1,-(uint)group->angle);
        crate = &Crate[group->iCrate];
        for(j = 0; j < group->nCrates; j++, crate++) {
           if ((crate->on != 0) && (crate->in_range != 0)) {
                type = GetCrateType(crate,0) + 1;
                 if ((u32)type > 1) {
                  min.x = ((s32)crate->dx * 0.5f);
                  min.y = crate->pos.y;
                  min.z = (crate->dz * 0.5f);
                  max.x = (min.x + 0.5f);
                  max.y = min.y + 0.5f;
                  max.z = (min.z + 0.5f);
                  if ((RayIntersectCuboid(&v0,&v1,&min,&max) != 0) && (temp_ratio < ratio)) {
                    face = temp_face;
                    ratio = temp_ratio;
                  }
                }
            }
        }
      }
  }
  temp_face = face;
  temp_ratio = (float)ratio;
  if (ratio < 1.0f) {
      return 1;
  }
  return 0;
}
