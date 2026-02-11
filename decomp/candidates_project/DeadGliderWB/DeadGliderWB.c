void DeadGliderWB(GLIDERSTRUCT *Glider) {
  if (Glider->Dead == 0) {
    ExplodeGlider(Glider);
  }
  return;
}
