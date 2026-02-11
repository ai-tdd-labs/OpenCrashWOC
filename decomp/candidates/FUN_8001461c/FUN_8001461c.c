typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_80105e60;

void FUN_8001461c(int param_1)

{
  undefined1 uVar1;
  
  *(undefined4 *)(param_1 + 0x24) = DAT_80105e60;
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_1 + 8);
  *(undefined1 *)(param_1 + 0x3f) = 0xff;
  *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(param_1 + 8);
  *(undefined1 *)(param_1 + 0x3e) = 0xff;
  if (((*(char *)(param_1 + 0x3a) == '\x06') || (*(char *)(param_1 + 0x3b) == '\x06')) ||
     ((*(char *)(param_1 + 0x3a) == '\0' && (*(char *)(param_1 + 0x3c) == '\x06')))) {
    uVar1 = 10;
  }
  else {
    uVar1 = 0;
  }
  *(undefined1 *)(param_1 + 0x58) = uVar1;
  *(undefined1 *)(param_1 + 0x42) = 0;
  *(undefined2 *)(param_1 + 0x76) = 0xffff;
  *(undefined1 *)(param_1 + 0x41) = 0;
  return;
}
