//NGC MATCH

void InitAI(void) {
  s32 i;
  
  LEVELAITYPES = 0;
  for(i = 0; i < 0x6b; i++) {
    if (CRemap[AIType[i].character] != -1) {
      LevelAIType[LEVELAITYPES++] = i;
    }
  }
  LEVELAICOUNT = 0;
  if (Level != 0x28) {
    LoadAI();
  }
  ResetAI();
  return;
}
