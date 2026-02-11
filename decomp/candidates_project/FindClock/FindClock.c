/*

struct creature_s * FindClock(void) {
  s32 i;

  for(i = 1; i < 9; i++) {
      if(Character[i].on != '\0' && ((Character[i].obj).character == 0x76)){
          return &Character[i];
      }
  }
  return NULL;
}
