//NGC MATCH

struct nugspline_s * NuSplineFindPartial(struct nugscn_s *scene,char *name,char *txt) {
  struct nugspline_s *spl;
  s32 i;
  
  spl = scene->splines;
  for(i = 0; i < scene->numsplines; i++) {
      if (strncasecmp(name,spl->name,strlen(name)) == 0) {
        strcpy(txt,spl->name);
        return spl;
      }
      spl++;
  }
  return NULL;
}
