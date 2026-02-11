typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_80105194;

void FUN_80009ab0(int param_1,int param_2)

{
  undefined4 uVar1;
  
  uVar1 = DAT_80105194;
  if (0 < param_2) {
    do {
      *(undefined1 *)(param_1 + 0x114) = 0xff;
      param_2 = param_2 + -1;
      *(undefined4 *)(param_1 + 0xe8) = uVar1;
      *(undefined4 *)(param_1 + 0xec) = uVar1;
      *(undefined4 *)(param_1 + 0xf0) = uVar1;
      *(undefined4 *)(param_1 + 0xe0) = uVar1;
      *(undefined4 *)(param_1 + 0xe4) = uVar1;
      *(undefined1 *)(param_1 + 0x11a) = 0;
      param_1 = param_1 + 0x11c;
    } while (0 < param_2);
    return;
  }
  return;
}
