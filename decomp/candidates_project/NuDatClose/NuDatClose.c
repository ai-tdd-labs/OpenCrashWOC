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

extern int datfiles;

void NuDatClose(struct nudathdr_s* ndh) {
	NuFileClose(ndh->fh);
	if (ndh->dfhandle) {
		datfiles[ndh->dfhandle - 0x800].used = 0;
	}
	if (ndh->intalloc) //managedmem
	{
		if (ndh->filetree != NULL)
		{
			NuMemFree(ndh->filetree);
		}
		NuMemFree(ndh);
	}
	return;
}
