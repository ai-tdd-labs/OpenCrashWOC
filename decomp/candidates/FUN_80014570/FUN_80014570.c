typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_8030860c;

undefined * FUN_80014570(int param_1,int param_2,int param_3,int param_4)

{
  undefined *puVar1;
  int iVar2;
  
  iVar2 = 0;
  puVar1 = &DAT_8030860c + *(short *)(param_1 + 0x10) * 0x90;
  if (0 < *(short *)(param_1 + 0x12)) {
    do {
      if (((*(short *)(puVar1 + 0x44) == param_2) && (*(short *)(puVar1 + 0x46) == param_3)) &&
         (*(short *)(puVar1 + 0x48) == param_4)) {
        return puVar1;
      }
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 0x90;
    } while (iVar2 < *(short *)(param_1 + 0x12));
  }
  return (undefined *)0x0;
}
