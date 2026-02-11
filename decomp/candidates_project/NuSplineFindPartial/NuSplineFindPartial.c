typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned int uint;
typedef int bool32;

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
