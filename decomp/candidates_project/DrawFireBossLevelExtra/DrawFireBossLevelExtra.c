#define ALIGN_ADDRESS(addr, al) (((s32)addr + (al-1)) & ~(al-1))

void DrawFireBossLevelExtra(void) {
  DrawFireBoss(&FireBoss);
  DrawJeepRocks();
  DrawVehMasks();
  return;
}
