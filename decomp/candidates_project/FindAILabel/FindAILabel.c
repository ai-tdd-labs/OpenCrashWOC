//NGC MATCH

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
