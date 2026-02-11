void InitBigGuns(void) {
  memset(BigGunList,0,0xff0);
  BigGunIndx = 0;
  InitBigGun(SetNuVecPntr(0.0,18.86,30.58),0.0,0,-65.0,-10.0,0.0,60.0);
  InitBigGun(SetNuVecPntr(0.0,-1.68,16.72),0.0,4,10.0,80.0,0.0,60.0);
  InitBigGun(SetNuVecPntr(-10.83,6.32,34.15),0.0,5,-65.0,-10.0,-90.0,60.0);
  InitBigGun(SetNuVecPntr(10.83,6.32,34.15),0.0,5,-65.0,-10.0,90.0,60.0);
  InitBigGun(SetNuVecPntr(0.03,23.71,2.48),0.0,0,-65.0,-10.0,180.0,60.0);
  InitBigGun(SetNuVecPntr(0.0,-25.5,-5.97),0.0,3,-30.0,30.0,180.0,60.0);
  InitBigGun(SetNuVecPntr(10.0,9.89,14.46),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(9.5,9.89,11.25),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(9.0,9.89,7.84),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(-10.0,9.89,14.46),0.0,2,-30.0,30.0,-90.0,30.0);
  InitBigGun(SetNuVecPntr(-9.5,9.89,11.25),0.0,2,-30.0,30.0,-90.0,30.0);
  InitBigGun(SetNuVecPntr(-9.0,9.89,7.84),0.0,2,-30.0,30.0,-90.0,30.0);
}
