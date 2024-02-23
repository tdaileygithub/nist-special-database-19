/*
# proc: allocmis - allocates and initializes an mis structure with specified
# proc:            attributes and length.
*/

#include <stdio.h>
#include <cstdlib>

#include "allocmis.h"
#include "syserr.h"
#include "imagutil.h"
#include "ihead.h"
#include "mishdr.h"

MIS* allocmis(char* filename, int depth, int bpi, int entw, int enth, int ent_alloc)
{
    MIS* mis;

    if ((mis = (MIS*)malloc(sizeof(MIS))) == NULL)
        syserr("allocmis", "malloc", "mis");
    mis->misw = entw;
    mis->mish = enth * ent_alloc;
    mis->misd = depth;
    mis->entw = entw;
    mis->enth = enth;
    mis->ent_num = 0;
    mis->ent_alloc = ent_alloc;
    /* the header id is to be filled in by the application or will */
    /* be automatically assigned within a call to writemisfile() */
    mis->head = buildmishdr(filename, depth, bpi, mis->misw, mis->mish,
        mis->entw, mis->enth, mis->ent_alloc, UNCOMP);

    mis->data = (unsigned char*)calloc(SizeFromDepth(mis->misw, mis->mish, depth),
        sizeof(unsigned char));
    if (mis->data == NULL)
        syserr("allocmis", "calloc", "mis->data");
    return(mis);
}
