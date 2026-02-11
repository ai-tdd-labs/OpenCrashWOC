typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_801083c4;

void FUN_80018fd8(int param_1)

{
  float fVar1;
  int unaff_r13;
  
  if (*(float *)(param_1 + 0xbc) != DAT_801083c4) {
    return;
  }
  fVar1 = *(float *)(unaff_r13 + -0x5ddc);
  *(float *)(param_1 + 0xbc) = fVar1;
  if (fVar1 <= *(float *)(param_1 + 0x118) * *(float *)(param_1 + 0x104) +
               *(float *)(param_1 + 0x6c)) {
    return;
  }
  *(float *)(param_1 + 0x6c) = fVar1 - *(float *)(param_1 + 0xec) * *(float *)(param_1 + 0x104);
  return;
}
