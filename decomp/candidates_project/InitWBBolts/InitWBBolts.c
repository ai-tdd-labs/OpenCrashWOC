void InitWBBolts(void) {
  memset(BoltList,0,0x1680);
  NuMtxSetScale(&BoltMtxA,SetNuVecPntr(1.5f,1.5f,1.5f));
  NuMtxSetScale(&BoltMtxB,SetNuVecPntr(8.0f,8.0f,8.0f));
}
