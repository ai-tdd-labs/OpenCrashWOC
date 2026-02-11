void ProcessSpaceStations(void) {
  s32 i;
  
  PlayerGlider.InTornado = 0;
  for(i = 0; i < 3; i++) {
    if (SpaceStationList[i].Active != 0) {
      ProcessSpaceStation(&SpaceStationList[i]);
    }
  }
}
