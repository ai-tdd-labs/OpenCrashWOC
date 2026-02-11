//NGC MATCH

s32 CombinationCharacterBC(char c0,char c1) {
    char *p;
    
    p = *j_bc;
    for (p = *j_bc; *p != '\0'; p+= 2) {
            if ((c0 == *p) && (c1 == p[1])) {
                return 1;
            }
    }
    return 0;
}
