#define rate_val 1.0f

void MakeLightDirection(s32 ix,float x,float y,float z) {
  cutdir[ix].x = x;
  cutdir[ix].y = y;
  cutdir[ix].z = z;
  NuVecNorm(&cutdir[ix],&cutdir[ix]);
  return;
}
