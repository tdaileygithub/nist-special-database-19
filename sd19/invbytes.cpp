#include <stdio.h>
#include <cstdlib>

#include "invbytes.h"
#include "syserr.h"

void inv_bytes(unsigned char* data,int nbytes)
{
    FILE* fp;
    unsigned char* lut, * ptr;
    int i;

    if ((fp = fopen(LUT_FILE, "r")) == NULL)
        syserr("inv_bytes", "fopen", LUT_FILE);
    lut = (unsigned char*)calloc(LUT_SIZE, sizeof(unsigned char));
    if (lut == NULL)
        syserr("inv_bytes", "calloc", "lut");
    fread(lut, sizeof(unsigned char), LUT_SIZE, fp);
    fclose(fp);
    ptr = data;
    for (i = 0; i < nbytes; i++) {
        *ptr = lut[*ptr];
        ptr++;
    }
    free(lut);
}
