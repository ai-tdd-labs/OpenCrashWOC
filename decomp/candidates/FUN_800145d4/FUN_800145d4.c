typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_80105e58;
extern int DAT_80105e5c;

void FUN_800145d4(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int unaff_r13;
  
  *(undefined4 *)(unaff_r13 + -0x7f00) = DAT_80105e58;
  uVar2 = DAT_80105e5c;
  uVar1 = DAT_80105e5c;
  if (*(int *)(unaff_r13 + -0x7d58) == 0x17) {
    uVar1 = *(undefined4 *)(unaff_r13 + -0x71dc);
  }
  *(undefined4 *)(unaff_r13 + -0x7200) = uVar1;
  *(undefined4 *)(unaff_r13 + -0x71fc) = uVar2;
  *(undefined4 *)(unaff_r13 + -0x71e8) = 0;
  *(undefined4 *)(unaff_r13 + -0x71ec) = 0;
  return;
}
