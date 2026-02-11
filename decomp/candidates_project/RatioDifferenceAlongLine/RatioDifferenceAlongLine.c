//NGC MATCH

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
