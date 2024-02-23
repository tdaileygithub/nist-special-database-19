/*
# proc: cut_image - returns the subimage from a binary bitmap that is
# proc:             byte aligned in origin and width.
# proc: cut_exact_image - returns the exact subimage requested from a binary
# proc:                   bitmap.
# proc: imagedup - takes an image of specified depth and duplicates its
# proc:            contents in a new image.
*/

/************************************************************/
/*         File Name: ImageOps.c                            */
/*         Package:   NIST Raster Utilities                 */
/*                                                          */
/*         Contents:  Cut_Image()                           */
/************************************************************/
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <cstdlib>

#include "defs.h"
#include "imageops.h"
#include "mis.h"
#include "syserr.h"
#include "imagutil.h"
#include "bincopy.h"

/************************************************************/
/*         Routine:   Cut_image()                           */
/*         Author:    Michael D. Garris                     */
/*         Date:      5/02/89                               */
/************************************************************/
/************************************************************/
/* Cut_image() takes a raw raster stream of given pixel     */
/* width and height, and clips a sub-raster image of "byte- */
/* aligned" origin and dimensions.                          */
/************************************************************/
unsigned char* cut_image(unsigned char* data, int sw, int sh, int* x, int y, int* w, int h)
{
    unsigned char* cdata, * cptr;
    int l, source_width, dest_width, horiz_offset;

    /* word align truncating to the left */
    *x = (int)(*x / WD_SIZE) * WD_SIZE;
    /* word align padding to the right */
    *w = (int)((double)ceil((double)(*w / WD_SIZE)) * WD_SIZE);
    /* byte width of source raster buffer */
    source_width = (int)(sw / BYTE_SIZE);
    /* byte width of sub-image */
    dest_width = (int)(*w / BYTE_SIZE);
    /* allocate sub-image buffer */
    cdata = (unsigned char*)malloc((dest_width * h) * sizeof(unsigned char));
    if (cdata == NULL)
        syserr("cut_image", "malloc", "cdata");
    cptr = cdata;
    /* horizontal byte offset into source data */
    horiz_offset = (int)(*x / BYTE_SIZE);
    /* foreach line to be clipped ... */
    for (l = y; l < (y + h); l++) {
        /* foreach byte to be clipped ... */
        memcpy(cptr, (data + (l * source_width) + horiz_offset), dest_width);
        cptr += dest_width;
    }
    /* return the sub-image */
    return(cdata);
}

/************************************************************/
unsigned char* cut_exact_image(unsigned char* data, int dw, int dh, int cx, int cy, int* cw, int ch)
{
    unsigned char* cdata;
    int aw, size;

    aw = ((int)ceil((double)((*cw) / 8.0))) << 3;
    size = SizeFromDepth(aw, ch, 1);
    cdata = (unsigned char*)calloc(size, sizeof(unsigned char));
    if (cdata == NULL)
        syserr("cut_exact_image", "calloc", "cdata");
    binary_subimage_copy(data, dw, dh, cdata, aw, ch, cx, cy, *cw, ch, 0, 0);
    *cw = aw;
    return(cdata);
}

/************************************************************/
unsigned char* imagedup(unsigned char* image, int width, int height, int depth)
{
    //unsigned char* allocate_image(), * dup = NULL;
    unsigned char* dup = NULL;

    dup = allocate_image(width, height, depth);
    memcpy(dup, image, SizeFromDepth(width, height, depth));
    return(dup);
}
