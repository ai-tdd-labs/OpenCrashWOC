/*

void ClockOff(void) {
  s32 i;
  
  for(i = 1; i < 9; i++) {
    if ((Character[i].on != '\0') && ((Character[i].obj).character == 0x76)) {
      KillItem(&Character[i].obj);
    }
  }
  clock_ok = 0;
  return;
}
