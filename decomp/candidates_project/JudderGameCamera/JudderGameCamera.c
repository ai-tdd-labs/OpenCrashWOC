#include "gamecode/camera.h"

/*

void JudderGameCamera(struct cammtx_s *cam,float time,struct nuvec_s *pos) {
    float d;

    if (time > cam->judder) {
        if (pos != NULL) {
            d = NuVecDist(&(player->obj).pos,pos,NULL);
            if (d < 10.0f) {
                cam->judder = time * ((10.0f - d) / 10.0f);
            }
        }
        else {
            cam->judder = time;
        }
    }
    return;
}
