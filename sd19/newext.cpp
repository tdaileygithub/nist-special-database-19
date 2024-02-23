/*
# proc: newext - takes a filename, stips off the rightmost extenstion, and
# proc:          appends a new extension.
*/

/************************************************************/
/*         Routine:   NewExt()                              */
/*         Author:    Michael D. Garris                     */
/*         Date:      11/27/89                              */
/************************************************************/
/*************************************************************/
/* Newext() is a destructive procedure which takes a filename*/
/* and strips off the rightmost extension (if one exists) and*/
/* appends the extension passed.                             */
/*************************************************************/
#include <stdio.h>
#include <string.h>

#include "newext.h"
#include "fatalerr.h"

void newext(char* file, int len,char* ext)
{
    char* cptr;

    cptr = file + strlen(file);
    while ((cptr != file) && (*cptr != '.'))
        cptr--;
    if (cptr == file) {
        if (strlen(file) == len)
            fatalerr("newext", "File manipulation exceeds allocated memory", NULL);
        cptr += strlen(file);
        *cptr++ = '.';
        *cptr = NULL;
    }
    else {
        cptr++;
        *cptr = NULL;
    }
    if ((strlen(file) + strlen(ext)) > len)
        fatalerr("newext", "File manipulation exceeds allocated memory", NULL);
    strcat(file, ext);
}
