typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_8031160c;

void FUN_800130b0(void)

{
  undefined1 *puVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int unaff_r13;
  
  iVar2 = 0;
  piVar3 = &DAT_8031160c;
  if (0 < *(int *)(unaff_r13 + -0x7ef8)) {
    do {
      iVar2 = iVar2 + 1;
      *(undefined1 *)(*piVar3 + 0x3a) = *(undefined1 *)(piVar3 + 1);
      *(undefined1 *)(*piVar3 + 0x3b) = *(undefined1 *)((int)piVar3 + 5);
      *(undefined1 *)(*piVar3 + 0x3c) = *(undefined1 *)((int)piVar3 + 6);
      iVar4 = *piVar3;
      puVar1 = (undefined1 *)((int)piVar3 + 7);
      piVar3 = piVar3 + 2;
      *(undefined1 *)(iVar4 + 0x3d) = *puVar1;
    } while (iVar2 < *(int *)(unaff_r13 + -0x7ef8));
  }
  *(undefined4 *)(unaff_r13 + -0x7ef8) = 0;
  return;
}
