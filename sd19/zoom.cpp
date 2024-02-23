/*
# proc: zoom - scales a binary bitmap up or down depending on the x & y factors
# proc:        passed.
# proc: zoom_x - scales a binary bitmap up or down based on the x factor
# proc:          passed.
# proc: zoom_y - scales a binary bitmap up or down based on the y factor
# proc:          passed.
# proc: enlarge_x - expand a binary bitmap's columns by the x factor passed.
# proc:
# proc: enlarge_y - expand a binary bitmap's rows by the y factor passed.
# proc:
# proc: shrink_x - down sample a binary bitmap's columns by the x factor
# proc:            passed.
# proc: shrink_y - down sample a binary bitmap's rows by the y factor passed.
# proc:
# proc: copy_cols - copies a single column of pixels to a range of column
# proc:             locations in a destination image.
# proc: copy_col - copies a column of pixels to a column location in a
# proc:            destination image.
# proc: copy_rows - copies a single row of pixels to a range of row locations
# proc:             in a destination image.
# proc: copy_row - copies a row of pixels to a row location in a destination
# proc:            image.
*/

#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <cstring>

#include "ihead.h"
#include "zoom.h"
#include "fatalerr.h"
#include "bits2bytes.h"
#include "imagutil.h"
#include "imageops.h"
#include "char2bin.h"

#define UCHAR_HACK 1

/************************************************************************/
void zoom(unsigned char** zmdata, int* zw, int* zh, unsigned char* bindata, int iw, int  ih, float xfctr, float yfctr)
{
    char* chardata;// , * allocate_image();
    char* xzmdata, * yzmdata;
    int xw, xh, yw, yh;

    if ((xfctr == 1.0) && (yfctr == 1.0)) {
        *zmdata = (unsigned char*)imagedup(bindata, iw, ih, 1);
        *zw = iw;
        *zh = ih;
    }
    else {
        /* if shrinking y, then do it first to mimimize length of col copies */
        if (yfctr <= 1.0) {

#if UCHAR_HACK
            chardata = (char*)allocate_image(iw, ih, 8);
            bits2bytes(bindata, (unsigned char*)chardata, iw * ih);
#else
            chardata = allocate_image(iw, ih, 8);
            bits2bytes(bindata, chardata, iw * ih);
#endif
            zoom_y(&yzmdata, &xw, &xh, chardata, iw, ih, yfctr);
            free(chardata);
            zoom_x(&xzmdata, &yw, &yh, yzmdata, xw, xh, xfctr);
            free(yzmdata);
            char2bin_exact(zmdata, zw, zh, xzmdata, yw, yh);
            free(xzmdata);
        }
        /* if enlarging y, then do it last to mimimize length of col copies */
        if (yfctr > 1.0) {
            
#if UCHAR_HACK
            chardata = (char*)allocate_image(iw, ih, 8);
            bits2bytes(bindata, (unsigned char*)chardata, iw * ih);
#else
            chardata = allocate_image(iw, ih, 8);
            bits2bytes(bindata, chardata, iw * ih);
#endif
            zoom_x(&xzmdata, &xw, &xh, chardata, iw, ih, xfctr);
            free(chardata);
            zoom_y(&yzmdata, &yw, &yh, xzmdata, xw, xh, yfctr);
            free(xzmdata);
            char2bin_exact(zmdata, zw, zh, yzmdata, yw, yh);
            free(yzmdata);
        }
    }
}

/************************************************************************/
void zoom_x(char** xzmdata, int* zw, int* zh, char* chardata, int iw, int  ih, float xfctr)
{
    if (xfctr == 1.0) {
#if UCHAR_HACK
        * xzmdata = (char*)imagedup((unsigned char*)chardata, iw, ih, 8);
#else
        * xzmdata = imagedup(chardata, iw, ih, 8);
#endif
        *zw = iw;
        *zh = ih;
    }
    else if (xfctr > 1.0)
        enlarge_x(xzmdata, zw, zh, chardata, iw, ih, xfctr);
    else
        shrink_x(xzmdata, zw, zh, chardata, iw, ih, xfctr);
}

/************************************************************************/
void zoom_y(char** yzmdata, int* zw, int* zh, char* chardata, int iw, int ih, float yfctr)
{
    if (yfctr == 1.0) {
#if UCHAR_HACK
        * yzmdata = (char*)imagedup((unsigned char*)chardata, iw, ih, 8);
#else
        * yzmdata = (char*)imagedup(chardata, iw, ih, 8);
#endif
        *zw = iw;
        *zh = ih;
    }
    else if (yfctr > 1.0)
        enlarge_y(yzmdata, zw, zh, chardata, iw, ih, yfctr);
    else
        shrink_y(yzmdata, zw, zh, chardata, iw, ih, yfctr);
}

/************************************************************************/
void enlarge_x(char** xzmdata, int* zw, int* zh, char* chardata, int iw, int ih, float xfctr)
{
    int from, to1, to2;
    float f_current;
    //char* allocate_image();

    *zw = (int)((xfctr * iw) + 0.5);
    *zh = ih;
    *xzmdata =(char*) allocate_image(*zw, *zh, 8);

    f_current = -1.0;
    from = 0;
    to1 = from;
    do {
        f_current += xfctr;
        to2 = (int)(f_current + 0.5);

        copy_cols((*xzmdata), *zw, *zh, to1, to2, chardata, iw, ih, from);

        from++;
        to1 = to2 + 1;
    } while (from < iw);
}

/************************************************************************/
void enlarge_y(char** yzmdata, int* zw, int* zh, char* chardata, int  iw, int ih, float yfctr)
{
    int from, to1, to2;
    float f_current;
    //char* allocate_image();

    *zw = iw;
    *zh = (int)((yfctr * ih) + 0.5);
    *yzmdata =(char*) allocate_image(*zw, *zh, 8);

    f_current = -1.0;
    from = 0;
    to1 = from;
    do {
        f_current += yfctr;
        to2 = (int)(f_current + 0.5);

        copy_rows((*yzmdata), *zw, *zh, to1, to2, chardata, iw, ih, from);

        from++;
        to1 = to2 + 1;
    } while (from < ih);
}

/************************************************************************/
void shrink_x(char** xzmdata, int* zw, int* zh, char* chardata, int iw, int ih, float xfctr)
{
    int from, to;
    float recip, f_current;
    //char* allocate_image();

    recip = 1.0f / xfctr;
    *zw = (int)((xfctr * iw) + 0.5);
    *zh = ih;
    *xzmdata =(char*) allocate_image(*zw, *zh, 8);

    f_current = 0.0;
    from = 0;
    to = from;
    do {
        copy_col((*xzmdata), *zw, *zh, to, chardata, iw, ih, from);
        f_current += recip;
        from = (int)(f_current + 0.5);
        to++;
    } while (to < *zw);
}

/************************************************************************/
void shrink_y(char** yzmdata, int* zw, int* zh, char* chardata, int iw, int ih, float yfctr)
{
    int from, to;
    float recip, f_current;
    //char* allocate_image();

    recip = 1.0f / yfctr;
    *zw = iw;
    *zh = (int)((yfctr * ih) + 0.5);
    *yzmdata =(char*)allocate_image(*zw, *zh, 8);

    f_current = 0.0;
    from = 0;
    to = from;
    do {
        copy_row((*yzmdata), *zw, *zh, to, chardata, iw, ih, from);
        f_current += recip;
        from = (int)(f_current + 0.5);
        to++;
    } while (to < *zh);
}

/************************************************************************/
void copy_cols(char* xzmdata, int zw, int zh, int to1, int to2, char* chardata, int iw, int ih, int from)
{
    int i;

    for (i = to1; i <= to2; i++)
        copy_col(xzmdata, zw, zh, i, chardata, iw, ih, from);
}

/************************************************************************/
void copy_col(char* xzmdata, int zw, int  zh, int  to, char* chardata, int iw, int ih, int  from)
{
    char* fptr, * tptr;
    int i;

    if (zh != ih)
        fatalerr("copy_col", "length of columns not equal", NULL);
    fptr = chardata + from;
    tptr = xzmdata + to;
    for (i = 0; i < zh; i++) {
        *tptr = *fptr;
        tptr += zw;
        fptr += iw;
    }
}

/************************************************************************/
void copy_rows(char* yzmdata, int zw, int  zh, int  to1, int  to2, char* chardata, int  iw, int  ih, int  from)
{
    int i;

    for (i = to1; i <= to2; i++)
        copy_row(yzmdata, zw, zh, i, chardata, iw, ih, from);
}

/************************************************************************/
void copy_row(char* yzmdata, int zw, int zh, int to, char* chardata, int iw, int ih, int from)
{
    char* fptr, * tptr;

    if (zw != iw)
        fatalerr("copy_row", "length of rows not equal", NULL);
    fptr = chardata + (from * iw);
    tptr = yzmdata + (to * zw);
    memcpy(tptr, fptr, zw);
}
