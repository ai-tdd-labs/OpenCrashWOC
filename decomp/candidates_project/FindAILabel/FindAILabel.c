typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned int uint;
typedef int bool32;

s32 FindAILabel(struct creatcmd_s *cmd,s32 i) {
  s32 j = 0;

  while(1) { 
    if (cmd->cmd == 0x90) {
        return 0; 
    }
    if((cmd->cmd == 0x8b) && (cmd->i == i)) {
        return j;
    }
    cmd++;
    j++;
  }
}
