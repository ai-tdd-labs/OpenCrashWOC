typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_8031160c;
extern int DAT_80311610;
extern int DAT_80311611;
extern int DAT_80311612;
extern int DAT_80311613;

void FUN_80013064(int param_1)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  int unaff_r13;
  
  iVar3 = *(int *)(unaff_r13 + -0x7ef8);
  if (0x1f < iVar3) {
    return;
  }
  iVar2 = iVar3 * 8;
  (&DAT_8031160c)[iVar3 * 2] = param_1;
  (&DAT_80311610)[iVar2] = *(undefined1 *)(param_1 + 0x3a);
  (&DAT_80311611)[iVar2] = *(undefined1 *)(param_1 + 0x3b);
  (&DAT_80311612)[iVar2] = *(undefined1 *)(param_1 + 0x3c);
  uVar1 = *(undefined1 *)(param_1 + 0x3d);
  *(int *)(unaff_r13 + -0x7ef8) = iVar3 + 1;
  (&DAT_80311613)[iVar2] = uVar1;
  return;
}
