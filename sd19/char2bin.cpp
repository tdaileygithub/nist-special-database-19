/*
# proc: char2bin_exact - converts an binary character image to a binary
# proc:                  bitmap byte-aligning the resulting image width.
*/

#include <stdio.h>
#include <math.h>
#include <cstdlib>

#include "char2bin.h"
#include "imagutil.h"
#include "syserr.h"

/************************************************************************/
void char2bin_exact(unsigned char** bindata, int* bw, int* bh, char* chardata, int w, int h)
{
    unsigned char* tptr, cmask;
    int memlen, groups_8;
    int i, j, k, bytes, rem;
    char* fptr;

    bytes = (int)ceil(w / 8.0);
    *bw = bytes << 3;
    *bh = h;
    memlen = SizeFromDepth(*bw, *bh, 1);
    if (((*bindata) = (unsigned char*)calloc(memlen, sizeof(char))) == NULL)
        syserr("char2bin_exact", "calloc", "bindata");
    groups_8 = w >> 3;
    rem = w % 8;
    fptr = chardata;
    tptr = (*bindata);
    for (i = 0; i < h; i++) {
        for (j = 0; j < groups_8; j++) {
            cmask = 0x00;
            for (k = 0; k < 7; k++) {
                cmask |= *fptr++;
                cmask <<= 1;
            }
            cmask |= *fptr++;
            *tptr++ = cmask;
        }
        if (rem != 0) {
            cmask = 0x00;
            for (k = 0; k < rem; k++) {
                cmask |= *fptr++;
                cmask <<= 1;
            }
            for (k = 0; k < 7 - rem; k++)
                cmask <<= 1;
            *tptr++ = cmask;
        }
    }
}
