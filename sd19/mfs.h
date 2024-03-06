#pragma once
#ifndef MFS_H_
#define MFS_H_

#define MAXMFSLENGTH 512

typedef struct mfsstruct {
	int alloc;
	int num;
	char** values;
} MFS;
#endif