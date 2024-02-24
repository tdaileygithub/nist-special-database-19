/*
# proc: writeihdrfile - writes the contents of an IHead structure and an
# proc:                 image memory to the specified file.
*/

/************************************************************/
/*         File Name: WriteRaster.c                         */
/*         Package:   Image Read Routines                   */
/*                                                          */
/*         Contents:  writeihdrfile()                       */
/************************************************************/
#include <cstdlib>
#include <stdio.h>

#include "fatalerr.h"
#include "grp4comp.h"
#include "imagutil.h"
#include "syserr.h"
#include "writhdr.h"
#include "writrast.h"

/************************************************************/
/*         Routine:   Writeihdrfile()                       */
/*         Author:    Michael D. Garris                     */
/*         Date:      4/26/89                               */
/*         Modifications:                                   */
/*           9/20/90   (Stan Janet) check return codes      */
/*           2/20/91   (MDG) compression capability         */
/************************************************************/
/************************************************************/
/* Writeihdrfile() writes a ihead structure and correspon-  */
/* ding image data to the specified file.                   */
/************************************************************/
void writeihdrfile(char* file, IHEAD* head, unsigned char* data)
{
    FILE* fp;
    int width, height, depth, code, filesize, n, compbytes;
    unsigned char* compdata; 

    /* reopen the image file for writing */
    fp = fopen(file, "wb");
    if (fp == NULL)
        syserr("writeihdrfile", "fopen", file);

    n = sscanf(head->width, "%d", &width);
    if (n != 1)
        fatalerr("writeihdrfile", "sscanf failed on width field", NULL);
    n = sscanf(head->height, "%d", &height);
    if (n != 1)
        fatalerr("writeihdrfile", "sscanf failed on height field", NULL);
    n = sscanf(head->depth, "%d", &depth);
    if (n != 1)
        fatalerr("writeihdrfile", "sscanf failed on depth field", NULL);
    n = sscanf(head->compress, "%d", &code);
    if (n != 1)
        fatalerr("writeihdrfile", "sscanf failed on compression code field", NULL);

    filesize = SizeFromDepth(width, height, depth);

    if (code == UNCOMP) {
        sprintf(head->complen, "%d", 0);
        writeihdr(fp, head);
        n = fwrite(data, 1, filesize, fp);
        if (n != filesize)
            syserr("writeihdrfile", "fwrite", file);
    }
    else {
        compdata = (unsigned char*)malloc(filesize * sizeof(unsigned char));
        if (compdata == NULL)
            syserr("writeihdrfile", "malloc", "compdata");
        switch (code) {
        case CCITT_G4:
            if (depth != 1)
                fatalerr("writeihdrfile",
                    "G4 compression requires a binary image.", NULL);
            grp4comp(data, filesize, width, height, compdata, &compbytes);
            break;
        default:
            fatalerr("writeihdrfile", "Unknown compression", NULL);
            break;
        }
        sprintf(head->complen, "%d", compbytes);
        writeihdr(fp, head);
        n = fwrite(compdata, 1, compbytes, fp);
        if (n != compbytes)
            syserr("writeihdrfile", "fwrite", file);
        free(compdata);
    }

    /* closes the new file */
    (void)fclose(fp);
}
