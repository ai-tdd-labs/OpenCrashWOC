//#include "gamecode/game_deb.h"

void AddGameDebris(s32 i,struct nuvec_s *pos) {
  s32 key;
  
  if (((NODEBRIS == 0) && ((uint)i < 0xaa)) && (GDeb[i].i != -1)) {
    key = -1;
    AddFiniteShotDebrisEffect(&key,GDeb[i].i,pos,1);
  }
  return;
}
