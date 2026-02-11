void ProcessSatellites(void) {
  s32 i;

  for(i = 0; i < 9; i++) {
    if (SatelliteList[i].Active != 0) {
      ProcessSatellite(&SatelliteList[i]);
    }
  }
  return;
}
