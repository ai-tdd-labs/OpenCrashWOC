/*

void ResetDeath(void) {
  struct rail_s* rail;

  death_begun = 0;
  Death = 0;
  rail = &Rail[6];

  if ((((rail->type != -1) && (Level == 0x25)) && (Hub == 5)) &&
     ((last_level != 0x26 && (last_level != -1)))) {
    StartTransporter(&death_obj,(struct nuvec_s *)(&(rail->pINPLAT)->pts[((rail->pINPLAT)->len + -1) * (s32)(rail->pINPLAT)->ptsize]));
    Death = 2;
    death_begun = 1;
  }
  return;
}
