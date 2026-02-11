void SetLights(struct nucolour3_s *vCOL0,struct nuvec_s *vDIR0,struct nucolour3_s *vCOL1,struct nuvec_s *vDIR1, struct nucolour3_s *vCOL2,struct nuvec_s *vDIR2,struct nuvec_s *vAMB) {
  NuLightSetDirectionalLights(vDIR0,vCOL0,vDIR1,vCOL1,vDIR2,vCOL2);
  NuLightSetAmbientLight(vAMB);
  return;
}
