//NGC MATCH

s32 CombinationCharacterBD(char c0,char c1) {
    char *p;
    
    p = *j_bd;

    for (p = *j_bd; *p != '\0'; p+= 2) {
        if ((c0 == *p) && (c1 == p[1])) {
            return 1;
        }
    }
    return 0;
}
