void InitTrail(void) {
  s32 loop;

  for(loop = 0; loop < 0x80; loop++) {
    trail[loop].pos1.x = -10000.0f;
  }
  trailpt = 0;
  trailair = 0;
}
