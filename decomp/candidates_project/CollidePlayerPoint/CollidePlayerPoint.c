s32 CollidePlayerPoint(struct nuvec_s *Pos,float Rad2,s32 Hits) {
  s32 OldHits;
  float TimerInc;
  
  OldHits = PlayerGlider.HitPoints;
  if (NuVecDistSqr(Pos,&PlayerGlider.Position,NULL) <= Rad2) {
     TimerInc = (float)Hits * 0.125f;
    if ((InvincibilityCHEAT == 0) && (VehicleLevelImmune == 0)) {
      PlayerGlider.HitPoints = PlayerGlider.HitPoints - Hits;
    }
    PlayerGlider.HitTimer += TimerInc;
    if (Hits != 0) {
      NewBuzz(&player->rumble,(BEENHITBIGBUZZTIME * 0x19) / Hits);
      NewRumble(&player->rumble,(BEENHITBIGRUMTIME * 0x19) / Hits);
    }
    if (4.5f <= PlayerGlider.HitTimer) {
      PlayerGlider.HitTimer = 4.5f;
    }
    if ((PlayerGlider.NextHitSoundTimer == 0.0f) ||
       (((PlayerGlider.HitPoints < 1 && (OldHits > 0)) || (4 < Hits)))) {
      MyGameSfx(0x5a,&PlayerGlider.Position,0x7fff);
      PlayerGlider.NextHitSoundTimer = GLIDERHITSOUNDFREQUENCY;
    }
    else {
      MyGameSfx(GLIDERHITSOUNDOTHERID,&PlayerGlider.Position,0x4fff);
    }
    if (PlayerGlider.HitPoints < 1) {
      PlayerGlider.HitPoints = 0;
      PlayerGlider.TerminalDive = 1;
      PlayerGlider.HitTimer = Level_DeadTime;
    }
    return 1;
  }
  return 0;
}
