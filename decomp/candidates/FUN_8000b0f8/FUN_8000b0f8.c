typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_80105278;

undefined4 FUN_8000b0f8(float *param_1,short *param_2)

{
  float fVar1;
  float fVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar3 = (int)*param_2;
  iVar7 = 1;
  if (1 < iVar3) {
    iVar4 = *(int *)(param_2 + 4);
    do {
      iVar6 = iVar7 * param_2[1];
      iVar7 = iVar7 + 1;
      iVar5 = iVar7;
      if (iVar7 == iVar3) {
        iVar5 = 1;
      }
      iVar5 = iVar5 * param_2[1];
      fVar2 = *(float *)(iVar4 + iVar6);
      fVar1 = *(float *)(iVar4 + iVar6 + 8);
      if ((*param_1 - fVar2) * (*(float *)(iVar4 + iVar5 + 8) - fVar1) +
          (param_1[2] - fVar1) * (fVar2 - *(float *)(iVar4 + iVar5)) < DAT_80105278) {
        return 0;
      }
    } while (iVar7 < iVar3);
  }
  return 1;
}
