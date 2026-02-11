typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_8030860c;

undefined4 FUN_80012e14(int param_1,undefined *param_2)

{
  undefined *puVar1;
  int iVar2;
  
  iVar2 = 0;
  puVar1 = &DAT_8030860c + *(short *)(param_1 + 0x10) * 0x90;
  if (0 < *(short *)(param_1 + 0x12)) {
    do {
      if ((((puVar1 != param_2) && (*(short *)(puVar1 + 0x44) == *(short *)(param_2 + 0x44))) &&
          (*(short *)(puVar1 + 0x48) == *(short *)(param_2 + 0x48))) &&
         (*(float *)(puVar1 + 8) < *(float *)(param_2 + 8))) {
        return 0;
      }
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 0x90;
    } while (iVar2 < *(short *)(param_1 + 0x12));
  }
  return 1;
}
