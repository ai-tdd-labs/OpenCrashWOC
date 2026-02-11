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

float RatioDifferenceAlongLine(float r0,float r1,struct nuvec_s *p0,struct nuvec_s *p1) {
  float dx;
  float dz;
  float r;
  float d;
  
  r = (r1 - r0);
  dx = ((p1->x - p0->x) * r);
  dz = ((p1->z - p0->z) * r);
  d = NuFsqrt(dx * dx + dz * dz);
  if (r < 0.0f) {
    d = -d;
  }
  return d;
}
