typedef int s32;

struct obj_s;

extern struct obj_s *pObj[64];
void CountGameObjects(void);

void RemoveGameObject(struct obj_s *obj) 
{
    s32 i;
    
    for (i = 0; i < 64; i++) 
    {
        if (pObj[i] == obj) 
        {
            pObj[i] = NULL;
            
            i = 64;
        }
    }
    
    CountGameObjects();
}
