typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

extern int DAT_80000034;
extern int DAT_800000f4;

undefined8 FUN_80003200(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  
  iVar1 = DAT_800000f4;
  if ((DAT_800000f4 != 0) && (*(int *)(DAT_800000f4 + 8) != 0)) {
    piVar4 = (int *)(DAT_800000f4 + *(int *)(DAT_800000f4 + 8));
    iVar2 = *piVar4;
    if (iVar2 != 0) {
      piVar3 = piVar4 + 1;
      iVar5 = iVar2;
      do {
        piVar4 = piVar4 + 1;
        *piVar4 = *piVar4 + iVar1;
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      DAT_80000034 = (uint)piVar3 & 0xffffffe0;
      return CONCAT44(iVar2,piVar3);
    }
  }
  return 0;
}
