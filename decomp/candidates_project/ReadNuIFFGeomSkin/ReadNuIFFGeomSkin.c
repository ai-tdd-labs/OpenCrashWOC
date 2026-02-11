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

static void ReadNuIFFGeomSkin(s32 handle,struct nugeom_s *geom) {
  s32 count;
  struct nuskin_s *skin;
  s32 bytes;
  s32 i;
  s32 j;

  count = NuFileReadInt(handle);
  if (count != 0) {
    if (NuFileReadChar(handle) != '\0') {
      bytes = geom->vtxcnt << 4;
      geom->vtxskininfo = NuMemAlloc(bytes);
      NuFileRead(handle,geom->vtxskininfo,bytes);
    }
    else {
        for (i = 0; i < count; i++) {
          skin = (struct nuskin_s *)NuMemAlloc(0x18);
          memset(skin,0,0x18);
          skin->vtxoffset = NuFileReadInt(handle);
          skin->vtxcnt = NuFileReadInt(handle);
          skin->mtxcnt = NuFileReadInt(handle);
          skin->mtxid = (s32 *)NuMemAlloc(skin->mtxcnt << 2);
            for (j = 0; j < skin->mtxcnt; j++) {
              skin->mtxid[j] = NuFileReadInt(handle);
            }
          bytes = (skin->vtxcnt * 4) * skin->mtxcnt;
          skin->weights = (f32 *)NuMemAlloc(bytes);
          NuFileRead(handle,skin->weights,bytes);
          NuGeomAddSkin(geom,skin);
        }
    }
  }
  return;
}
