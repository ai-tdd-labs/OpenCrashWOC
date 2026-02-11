typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef unsigned int uint;
typedef int bool32;
#ifndef NULL
#define NULL ((void*)0)
#endif

struct nuvec_s { f32 x; f32 y; f32 z; };
struct nuvec4_s { f32 x; f32 y; f32 z; f32 w; };
struct nuinstance_s;
struct nugscn_s;
struct nugobj_s;
struct creature_s;
struct creatcmd_s;
struct numtx_s;
struct nuwindgrp_s;

void SetIdentity(struct _GSMATRIX *d) {
  d->_11 = 1.0f;
  d->_12 = 0.0f;
  d->_13 = 0.0f;
  d->_14 = 0.0f;
  d->_21 = 0.0f;
  d->_22 = 1.0f;
  d->_23 = 0.0f;
  d->_24 = 0.0f;
  d->_31 = 0.0f;
  d->_32 = 0.0f;
  d->_33 = 1.0f;
  d->_34 = 0.0f;
  d->_41 = 0.0f;
  d->_42 = 0.0f;
  d->_43 = 0.0f;
  d->_44 = 1.0f;
  return;
}
