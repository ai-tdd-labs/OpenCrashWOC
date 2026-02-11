typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned int uint;
typedef int bool32;

void FUN_80080c6c(int param_1, int *param_2) {
    int iVar1;

    if (*param_2 != 0) {
        if (*param_2 == param_1) {
            *param_2 = *(int *)(param_1 + 0x10);
        } else {
            do {
                iVar1 = *param_2;
                param_2 = (int *)(iVar1 + 0x10);
                if (*(int *)(iVar1 + 0x10) == 0) {
                    goto LAB_80080cb0;
                }
            } while (*(int *)(iVar1 + 0x10) != param_1);
            *(int *)(iVar1 + 0x10) = *(int *)(param_1 + 0x10);
        }
    }
LAB_80080cb0:
    *(int *)(param_1 + 0x10) = 0;
}
