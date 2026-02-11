typedef int s32;

struct obj_s;

extern s32 GAMEOBJECTCOUNT;
extern struct obj_s *pObj[64];

void CountGameObjects() 
{
    s32 i;

    for (i = 64; i > 0; i--) 
    { 
        if (pObj[i - 1] != NULL) 
        {
            break;
        }
    }
    
    GAMEOBJECTCOUNT = i; 
}
