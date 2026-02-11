s32 GetCurrentWeatherResearchObjectives(void) {
  s32 i;
  s32 j;
  
  for(i = 0, j = 0; i < 0xc; i++) {
    if ((BigGunList[i].Active != 0) && (BigGunList[i].Action != 2)) {
      j++;
    }
  }
  return j;
}
