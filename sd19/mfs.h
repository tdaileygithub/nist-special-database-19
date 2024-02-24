#pragma once

#define MAXMFSLENGTH 512

typedef struct mfsstruct {
	int alloc;
	int num;
	char** values;
} MFS;