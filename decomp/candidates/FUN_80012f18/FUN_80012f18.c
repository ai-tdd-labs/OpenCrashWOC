typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_8030860c;

FUN_80012f18(undefined4 param_1,int param_2,undefined *param_3,int param_4,int param_5,int param_6)

{
  undefined *puVar1;
  int iVar2;
  
  iVar2 = 0;
  puVar1 = &DAT_8030860c + *(short *)(param_2 + 0x10) * 0x90;
  if (0 < *(short *)(param_2 + 0x12)) {
    do {
      if ((((puVar1 != param_3) && (*(char *)(param_6 + iVar2) == '\x01')) &&
          (*(short *)(puVar1 + 0x44) == param_4)) && (*(short *)(puVar1 + 0x48) == param_5)) {
        return 1;
      }
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 0x90;
    } while (iVar2 < *(short *)(param_2 + 0x12));
  }
  return 0;
}
