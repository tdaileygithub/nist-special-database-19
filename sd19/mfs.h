#pragma once

#define MAXMFSLENGTH 512

typedef struct mfsstruct {
	int alloc;
	int num;
	char** values;
} MFS;

//extern MFS* allocmfs();
//extern void appndmfs();
//extern void freemfs();
//extern MFS* readmfsfile();
//extern int readmfshdr();
//extern void mfs2int2();
