#pragma once

#define MAXMITLENGTH 1034
#define MIT_EXT         "mit"

typedef struct mitstruct{
   int alloc;
   int num;
   char **values;
} MIT;

extern MIT *allocmit();
extern void appendmit();
extern MIT *buildmit();
extern char *extractmit();
extern void fragmit();
extern void freemit();
extern MFS *mfsfrmit();
extern MIS *misfrmit();
extern void printmit();
extern MIT *readmitfile();
extern void writemitfile();
