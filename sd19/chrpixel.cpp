/*
# proc: pixelcnt - counts the number of black pixels in a binary character
# proc:            image.
# proc: shift_row - takes a row of character pixels and shifts them left or
# proc:             right by a given number of pixel into an output row.
# proc: left_pix - locates the first nonzero pixel left-to-right in a given
# proc:            row of character pixels and returns -1 if no pixel is found.
# proc: char_top - locates the first row of pixels top-to-bottom in a character
# proc:            image that contains a nonzero pixel and returns -1 if no
# proc:            row is found.
# proc: char_bottom - locates the first row of pixels bottom-to-top in a
# proc:               character image that contains a nonzero pixel and
# proc:               returns -1 if no row is found.
*/

#include <stdio.h>
#include <cstdlib>
#include <string.h>

#include "chrpixel.h"

/*******************************************************************/
int pixelcnt(char* cdata, int w, int h)
{
	int i, count, size;
	char* cptr;

	count = 0;
	size = w * h;
	cptr = cdata;
	for (i = 0; i < size; i++)
		if (cptr[i])
			count++;
	return(count);
}

/*************************************************************************/
void shift_row(unsigned char* irow, unsigned char* orow, int w, int sval)
{
	if (sval == 0) {
		memcpy(orow, irow, w);
		return;
	}
	if (abs(sval) >= w) {
		memset(orow, NULL, w);
		return;
	}
	if (sval > 0) {
		memset(orow, NULL, w);
		memcpy(orow + sval, irow, w - sval);
		return;
	}
	if (sval < 0) {
		memset(orow, NULL, w);
		memcpy(orow, irow - sval, w + sval);
		return;
	}
}

/*************************************************************************/
int left_pix(unsigned char* row, int w)
{
	int i;
	unsigned char* rptr;

	rptr = row;
	for (i = 0; i < w; i++) {
		if (*rptr != 0)
			return(i);
		else
			rptr++;
	}
	return(-1);
}

/*************************************************************************/
int char_top(unsigned char* cdata, int w, int h)
{
	int r, c;
	unsigned char* cptr;

	cptr = cdata;
	for (r = 0; r < h; r++) {
		for (c = 0; c < w; c++) {
			if (*cptr != 0)
				return(r);
			else
				cptr++;
		}
	}
	return(-1);
}

/*************************************************************************/
int char_bottom(unsigned char* cdata, int w, int h)
{
	int len, r, c;
	unsigned char* cptr;

	len = w * h;
	cptr = cdata + len - 1;
	for (r = h - 1; r >= 0; r--) {
		for (c = 0; c < w; c++) {
			if (*cptr != 0)
				return(r);
			else
				cptr--;
		}
	}
	return(-1);
}
