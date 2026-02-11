//NGC MATCH

s32 FindAIType(char *name,s32 points) {
  s32 i;
  
  for (i = 0; i < 0x6b; i++) {
      if ((strcmp(name,AIType[i].name) == 0 && (points == AIType[i].points)) && (points < 9)) {
          return i;
      }
  }
  return -1;
}
