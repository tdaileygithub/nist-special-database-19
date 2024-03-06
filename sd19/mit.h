#pragma once
#ifndef MIT_H
#define MIT_H

#define MAXMITLENGTH	1034
#define MIT_EXT         "mit"

typedef struct mitstruct {
	int alloc;
	int num;
	char** values;
} MIT;
#endif