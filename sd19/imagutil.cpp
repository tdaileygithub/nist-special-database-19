/*
# proc: PixPerByte - returns the pixel per byte factor given the depth of
# proc:              a pixel in an image.
# proc: SizeFromDepth - computes the byte size of an image based on the
# proc:                 image's pixel width, pixel height, and pixel depth.
# proc: allocate_image - allocates an image of specified width, height, and
# proc:                  depth.
*/

/************************************************************/
/*         File Name: ImageUtil.c                           */
/*         Package:   Image Utilities                       */
/*                                                          */
/*         Contents:  Allocate_image()                      */
/*                    PixPerByte()                          */
/*                    SizeFromDepth()                       */
/************************************************************/
#include <cstdlib>
#include <math.h>
#include <memory.h>
#include <stdio.h>

#include "fatalerr.h"
#include "imagutil.h"
#include "syserr.h"

/************************************************************/
/*         Routine:   PixPerByte()                          */
/*         Author:    Michael D. Garris                     */
/*                    Darrin Dimmick                        */
/*         Date:      3/07/90                               */
/*         Modifications:                                   */
/*           9/20/90    (Stan Janet) error message          */
/************************************************************/
/************************************************************/
/* PixPerByte() takes the pixel depth of an image and       */
/* returns the corresponding pixperbyte factor.             */
/************************************************************/
float PixPerByte(int depth)
{
	float pixperbyte;

	switch (depth) {
	case 1:
		pixperbyte = 8.0;
		break;
	case 2:
		pixperbyte = 4.0;
		break;
	case 4:
		pixperbyte = 2.0;
		break;
	case 8:
		pixperbyte = 1.0;
		break;
	case 16:
		pixperbyte = 0.5;
		break;
	case 32:
		pixperbyte = 0.25;
		break;
	default:
		fatalerr("PixPerByte",
			"depth is not a power of 2 between 1 and 32 (inclusive)",
			NULL);
	}
	return pixperbyte;
}

/************************************************************/
/*         Routine:   SizeFromDepth()                       */
/*         Author:    Michael D. Garris                     */
/*                    Darrin Dimmick                        */
/*         Date:      2/20/90                               */
/*         Modifications:                                   */
/*           9/20/90   (Stan Janet) add ceil() [bug]        */
/************************************************************/
/************************************************************/
/* SizeFromDepth() takes the pixel width of an image scan   */
/* line along with the pixel height of the image, and using */
/* the argument "depth" computes the length of the image in */
/* bytes.                                                   */
/************************************************************/
int SizeFromDepth(int pixwidth, int pixheight, int depth)
{
	int filesize;
	float pixperbyte;//, PixPerByte();

	pixperbyte = PixPerByte(depth);
	filesize = ((int)ceil(pixwidth / pixperbyte) * pixheight);
	return filesize;
}

/************************************************************/
/*         Routine:   allocate_image()                      */
/*         Author:    Darrin L. Dimmick                     */
/*         Date:      3/02/90                               */
/*         Modifications:                                   */
/************************************************************/
/************************************************************/
/* Allocate_image() takes the pixel width, height and depth */
/* of an image and uses SizeFromDepth to calloc the         */
/* requested memory                                         */
/************************************************************/

unsigned char* allocate_image(int width, int height, int depth)
{
	char error[2048];
	unsigned char* image = NULL;

	if (width < 1) {
		sprintf(error, "width = %d", width);
		fatalerr("allocate_image", "Invalid dimension", error);
	}
	if (height < 1) {
		sprintf(error, "height = %d", height);
		fatalerr("allocate_image", "Invalid dimension", error);
	}
	if (depth < 1) {
		sprintf(error, "depth = %d", depth);
		fatalerr("allocate_image", "Invalid dimension", error);
	}
	if ((image = (unsigned char*)calloc(SizeFromDepth(width, height, depth),
		sizeof(unsigned char))) == NULL)
		syserr("allocate_image", "calloc", "image");
	return(image);
}

