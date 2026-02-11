void NewLanguage(s32 l) {
  PData[0].name = tSNEAKSHOES[(u8)l];
  PData[0].description = (s32 *)tSNEAKSHOESTEXT[(u8)l];
  PData[1].name = tDOUBLEJUMP[(u8)l];
  PData[1].description = (s32 *)tDOUBLEJUMPTEXT[(u8)l];
  PData[2].name = tDEATHTORNADOSPIN[(u8)l];
  PData[2].description = (s32 *)tDEATHTORNADOSPINTEXT[(u8)l];
  PData[3].name = tFRUITBAZOOKA[(u8)l];
  PData[3].description = (s32 *)tFRUITBAZOOKATEXT[(u8)l];
  PData[4].name = tSPEEDSHOES[(u8)l];
  PData[4].description = (s32 *)tSPEEDSHOESTEXT[(u8)l];
  PData[5].name = tSUPERBELLYFLOP[(u8)l];
  PData[5].description = (s32 *)tSUPERBELLYFLOPTEXT[(u8)l];
  Game.language = l;
  DefaultTimeTrialNames(0);
  return;
}
