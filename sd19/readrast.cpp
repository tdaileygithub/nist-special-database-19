/*
# proc: ReadBinaryRaster - reads the contents of a binary IHead image file
# proc:                    into an IHead structure and image memory.
*/

/************************************************************/
/*         File Name: ReadRast.c                            */
/*         Package:   Image Read Routines                   */
/*                                                          */
/*         Contents:  ReadBinaryRaster()                    */
/************************************************************/
#include <cstdlib>
#include <math.h>
#include <stdio.h>

#include "fatalerr.h"
#include "getnset.h"
#include "grp4decomp.h"
#include "ihead.h"
#include "imagutil.h"
#include "invbytes.h"
#include "readihdr.h"
#include "readrast.h"
#include "syserr.h"

/************************************************************/
/*         Routine:   ReadBinaryRaster()                    */
/*         Author:    Michael D. Garris                     */
/*         Date:      4/28/89                               */
/*         Modifications:                                   */
/*           8/90 Stan Janet                                */
/*                     only malloc 1 buffer if data is not  */
/*                        compressed                        */
/*                     free() up temp buffer                */
/*           9/20/90 Stan Janet                             */
/*                     check return codes                   */
/*           1/11/91 Stan Janet                             */
/*                     put filename in error messages       */
/************************************************************/
/************************************************************/
/* ReadBinaryRaster() reads in a "headered" binary raster   */
/* file and returns an ihead structure, raster data, and    */
/* integer file specs.                                      */
/************************************************************/

void ReadBinaryRaster(char* file, IHEAD** head, unsigned char** data, int* bpi, int* width, int* height)
{
	FILE* fp;
	//IHEAD* readihdr();
	IHEAD* ihead;
	int outbytes, depth, comp, filesize, complen, n;
	unsigned char* indata, * outdata;// , * allocate_image();

	/* open the image file */
	fp = fopen(file, "rb");
	if (fp == NULL)
		syserr("ReadBinaryRaster", file, "fopen");

	/* read in the image header */
	(*head) = readihdr(fp);
	ihead = *head;

	depth = get_depth(ihead);
	if (depth != 1)
		fatalerr("ReadBinaryRaster", file, "not a binary file");
	(*width) = get_width(ihead);
	(*height) = get_height(ihead);
	(*bpi) = get_density(ihead);
	comp = get_compression(ihead);
	complen = get_complen(ihead);

	/* allocate a raster data buffer */
	filesize = SizeFromDepth(*width, *height, depth);
	outdata = allocate_image(*width, *height, depth);

	/* read in the raster data */
	if (comp == UNCOMP) {   /* file is uncompressed */
		n = fread(outdata, 1, filesize, fp);
		if (n != filesize) {
			(void)fprintf(stderr,
				"ReadBinaryRaster: %s: fread returned %d (expected %d)\n",
				file, n, filesize);
			exit(1);
		} /* IF */
	}
	else {
		indata = (unsigned char*)malloc(complen);
		if (indata == NULL)
			syserr("ReadBinaryRaster", file, "malloc (indata)");

		n = fread(indata, 1, complen, fp); /* file compressed */
		if (n != complen) {
			if (feof(fp)) {
				puts("End-of-File reached.");
				printf("Total number of bytes read: %d\n", n);
			}

			perror("The following error occurred");
			(void)fprintf(stderr,
				"ReadBinaryRaster: %s: fread returned %d (expected %d)\n",
				file, n, complen);
		} /* IF */
	}

	switch (comp) {
	case CCITT_G4:
		if ((*head)->sigbit == LSBF) {
			inv_bytes(indata, complen);
			(*head)->sigbit = MSBF;
			(*head)->byte_order = HILOW;
		}
		grp4decomp(indata, complen, *width, *height, outdata, &outbytes);
		set_compression(ihead, UNCOMP);
		set_complen(ihead, 0);
		free((char*)indata);
		break;
	case UNCOMP:
		break;
	default:
		fatalerr("ReadBinaryRaster", file, "Unsupported compression code");
		break;
	}

	*data = outdata;
	/* close the image file */
	(void)fclose(fp);
}
