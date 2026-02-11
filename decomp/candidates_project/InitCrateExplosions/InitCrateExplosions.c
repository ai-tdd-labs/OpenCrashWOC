//NGC MATCH

void InitCrateExplosions(void) {
  s32 i;
  
  iBOXEXP = 0;
  for(i = 0; i < 0x10; i++) {
    BoxExpList[i].time = 0;
  }
  return;
}
