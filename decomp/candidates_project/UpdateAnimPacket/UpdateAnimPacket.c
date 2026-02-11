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

void UpdateAnimPacket(struct CharacterModel *mod,struct anim_s *anim,float dt,float xz_distance) {
    float t;
    
    if ((mod == NULL) || (anim == NULL)) {
        return;
    }
    
    if (anim->blend != 0) {
        anim->blend_frame++;
        if (anim->blend_frame == anim->blend_frames) {
            anim->blend = 0;
            anim->action = anim->blend_dst_action;
            anim->anim_time = anim->blend_dst_time;
        }
    }
    else if (anim->newaction != anim->oldaction) {
        if ((((anim->oldaction != -1) && (anim->newaction != -1)) && (mod->anmdata[anim->oldaction] != NULL)) && (((mod->anmdata[anim->newaction] != NULL &&
        (mod->animlist[anim->oldaction]->blend_out_frames > 1)) && (mod->animlist[anim->newaction]->blend_in_frames > 1)))) {
            anim->blend = 1;
            anim->blend_src_action = anim->oldaction;
            anim->blend_dst_action = anim->newaction;
            anim->blend_src_time = anim->anim_time;
            if ((((mod->animlist[anim->oldaction]->flags & 1) != 0) && ((mod->animlist[anim->newaction]->flags & 1) != 0))
            && ((mod->animlist[anim->oldaction]->speed == mod->animlist[anim->newaction]->speed && (mod->anmdata[anim->oldaction]->time == mod->anmdata[anim->newaction]->time)))) {
                anim->blend_dst_time = anim->anim_time;
            }
            else {
                anim->blend_dst_time = 1.0f;
            }
            
            if (((mod->character == 0) && (PLAYERCOUNT != 0)) && (player->used != 0)) {
                if (anim->blend_dst_action == 3) {
                    anim->blend_dst_time = ((float)(player->crouch_pos) * (mod->anmdata[anim->blend_dst_action]->time - 1.0f)) / (float)(player->OnFootMoveInfo->CROUCHINGFRAMES);
                }
                else if (anim->blend_dst_action == 5) {
                    anim->blend_dst_time = ((float)(player->OnFootMoveInfo->CROUCHINGFRAMES - player->crouch_pos) * (mod->anmdata[anim->blend_dst_action]->time - 1.0f)) / (float)player->OnFootMoveInfo->CROUCHINGFRAMES;
                }
            }
            anim->blend_frame = 0;
            anim->blend_frames = (u16)mod->animlist[anim->newaction]->blend_in_frames;
            if (mod->animlist[anim->oldaction]->blend_out_frames < anim->blend_frames) {
                anim->blend_frames = (u16)mod->animlist[anim->oldaction]->blend_out_frames;
            }
        } else {
            anim->action = anim->newaction;
            anim->anim_time = 1.0f;   
            anim->blend = 0;
        }
    } else {
        anim->action = anim->newaction;
        anim->blend = 0;
    }
    anim->flags = 0;
    if (anim->blend != 0) {
        if ((mod->anmdata[anim->blend_src_action] == NULL) || (mod->anmdata[anim->blend_dst_action] == NULL)) {
            return;
        }
        t = dt * mod->animlist[anim->blend_src_action]->speed;
        if ((mod->animlist[anim->blend_src_action]->flags & 0x10) != 0) {
            t *= xz_distance * 10.0f;
        }
        anim->blend_src_time += t;
        t = mod->anmdata[anim->blend_src_action]->time;
        if (anim->blend_src_time > t) {
            if ((mod->animlist[anim->blend_src_action]->flags & 1) != 0) {
                anim->blend_src_time -= (t - 1.0f);
            }
            else {
                anim->blend_src_time = t;
            }
        }
        t = dt * mod->animlist[anim->blend_dst_action]->speed;
        if ((mod->animlist[anim->blend_dst_action]->flags & 0x10) != 0) {
            t *= xz_distance * 10.0f;
        }
        anim->blend_dst_time += t;
        t = mod->anmdata[anim->blend_dst_action]->time;
        if (anim->blend_dst_time > t) {
            if ((mod->animlist[anim->blend_dst_action]->flags & 1) != 0) {
                anim->flags = anim->flags | 2;
                anim->blend_dst_time -= (t - 1.0f);
                return;
            }
            anim->blend_dst_time = t;
            anim->flags = anim->flags | 1;
        }
    }
    else {
        if (mod->anmdata[anim->action] == NULL) {
            return;
        }
        t = dt * mod->animlist[anim->action]->speed;
        if ((mod->animlist[anim->action]->flags & 0x10) != 0) {
            t *= xz_distance * 10.0f;
        }
        anim->anim_time += t;
        t = mod->anmdata[anim->action]->time;
        if (anim->anim_time > t) {
            if ((mod->animlist[anim->action]->flags & 1) != 0) {
                anim->flags = 2;
                anim->anim_time -= (t - 1.0f);
                return;
            }
            anim->anim_time = t;
            anim->flags = 1;
        }
    }
}
