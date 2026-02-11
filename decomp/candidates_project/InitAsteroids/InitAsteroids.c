void InitAsteroids(void) {
  s32 i;
  
  memset(&AsteroidList,0,0x1db0);
  for(i = 0; i < 0x1e; i++) {
    InitAsteroid(0);
  }
}
