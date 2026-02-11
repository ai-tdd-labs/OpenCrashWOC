void InitGunBoat(struct nuvec_s *Pos,float AngleY,s32 Character) {
  GUNBOATSTRUCT* GunBoat;
  s32 CharacterId;

  GunBoat = &GunBoatList[Character];
  if (GunBoat->Active != 0) {
    return;
  }
  memset(&GunBoatList[Character],0,0x134);
  GunBoat->Character = Character;
  GunBoat->Position = *Pos;
  GunBoat->AngleY = AngleY;
  GunBoat->Action = 1;
  GunBoat->LastAction = -1;
  GunBoat->FireAngleX = 45.0;
  switch(Character) {
      case 0:
          CharacterId = 0x68;
      break;
      case 1:
          CharacterId = 0x69;
      break;
      case 2:
          CharacterId = 0x67;
      break;
      default:
          CharacterId = 0x66;
      break;
  }
  if (MyInitModelNew(&GunBoat->MainDraw,CharacterId,0x22,0,NULL,&GunBoat->Position) == 0) {
    GunBoat->Character = -1;
  }
  GunBoat->Active = 1;
}
