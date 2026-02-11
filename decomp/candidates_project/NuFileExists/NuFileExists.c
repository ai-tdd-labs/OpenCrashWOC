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

extern int FILE;
extern int filelength;
extern int fileoffset;
extern int filep;
extern int name;
extern int seekoffset;

s32 NuFileExists(char* filename) {
    FILE* filep; //__sFILE*
    char name[128] = "";
    char tmp;

    //strcpy(name, "/");
    seekoffset = 0; //fopen NGC
	strcat(name, filename);
	filep = fopen(name, "r");
	printf("check F.E. \n");
	if (filep != NULL) {
            //skip SDK check
        fileoffset = 0;
        filelength = 1;
            //
		fclose(filep);
        printf("file exist! %s \n", filename);
        return 1;
	}
	printf("file not exist %s \n", filename);
	return 0;
}
