s32 CollideWithBattleShips(struct nuvec_s *ObjPos,float Rad) {
  s32 i;

  for (i = 0; i < 6; i++) {
    if ((BattleShipList[i].Active != 0 && (CollideWithBattleShip(ObjPos,&BattleShipList[i],Rad) != 0))) {
        return 1;
    }
  }
  return 0;
}
