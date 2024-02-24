/*
# proc: compute_hist - computes a horizontal or vertical spatial histogram
# proc:                from a binary bitmap.
# proc: compute_x_hist - computes a horizontal spatial histogram from a
# proc:                  binary bitmap.
# proc: compute_y_hist - computes a vertical spatial histogram from a
# proc:                  binary bitmap.
*/

/*************************************************************/
/*                                                           */
/*       File Name:    Histogram.c                           */
/*       Package:      Spatial Histogram Computations        */
/*                                                           */
/*       Contents:     Compute_Hist()                        */
/*                     Compute_X_Hist()                      */
/*                     Compute_Y_Hist()                      */
/*************************************************************/
#include <cstdlib>
#include <math.h>
#include <memory.h>
#include <stdio.h>

#include "bits.h"
#include "defs.h"
#include "fatalerr.h"
#include "histgram.h"
#include "masks.h"
#include "syserr.h"

/************************************************************/
/*         Routine:   Compute_Hist()                        */
/*         Author:    Michael D. Garris                     */
/*         Date:      7/21/89                               */
/************************************************************/
/*************************************************************/
/* Compute_hist() Serves as a switch for orthogonal hist-    */
/* ogram orientations.                                       */
/*************************************************************/
void compute_hist(unsigned char* data, int width, int height, int orient, int** bins, int* len)
{
	switch (orient) {
	case Y_HIST:
		compute_y_hist(data, width, height, bins, len);
		break;
	case X_HIST:
		compute_x_hist(data, width, height, bins, len);
		break;
	default:
		fatalerr("comput_hist", "Unknown histogram orientation", NULL);
	}
}

/************************************************************/
/*         Routine:   Compute_X_Hist()                      */
/*         Author:    Michael D. Garris                     */
/*         Date:      7/21/89                               */
/************************************************************/
/*************************************************************/
/* Compute_x_hist() Computes an X-oriented histogram on the  */
/* defined rectangular image region passed.                  */
/*************************************************************/
void compute_x_hist(unsigned char* data, int width, int height, int** bins, int* len)
{
	FILE* fp;
	int y, x_byte, bnum, bytewidth, bytelines, memsize;
	short* lut;

	if ((fp = fopen(BITLUT, "r")) == NULL)
		syserr("compute_x_hist", "fopen", BITLUT);
	lut = (short*)malloc(LUTLEN * sizeof(short));
	if (lut == NULL)
		syserr("compute_x_hist", "malloc", "lut");
	fread(lut, sizeof(short), LUTLEN, fp);
	fclose(fp);
	bytewidth = width / 8;
	memsize = height * sizeof(int);
	(*bins) = (int*)malloc(memsize);
	if ((*bins) == NULL)
		syserr("compute_x_hist", "malloc", "bins");
	memset((*bins), NULL, memsize);
	bnum = 0;
	for (y = 0; y < height; y++) {
		bytelines = y * bytewidth;
		for (x_byte = 0; x_byte < bytewidth; x_byte++) {
			(*bins)[bnum] += lut[(int)(*(data + bytelines + x_byte))];
		}
		bnum++;
	}
	*len = bnum;
}

/************************************************************/
/*         Routine:   Compute_Y_Hist()                      */
/*         Author:    Michael D. Garris                     */
/*         Date:      7/21/89                               */
/************************************************************/
/*************************************************************/
/* Compute_y_hist() Computes a Y-oriented histogram on the   */
/* defined rectangular image region passed.                  */
/*************************************************************/
void compute_y_hist(unsigned char* data, int width, int height, int** bins, int* len)
{
	int bytewidth, bytelines, y, x_byte, bit, bnum, memsize;
	unsigned char byte;

	bytewidth = width / 8;
	memsize = width * sizeof(int);
	(*bins) = (int*)malloc(memsize);
	if ((*bins) == NULL)
		syserr("compute_y_hist", "malloc", "bins");
	memset((*bins), NULL, memsize);
	for (y = 0; y < height; y++) {
		bnum = 0;
		bytelines = y * bytewidth;
		for (x_byte = 0; x_byte < bytewidth; x_byte++) {
			byte = *(data + bytelines + x_byte);
			for (bit = 7; bit >= 0; bit--) {
				((*bins)[bnum]) += get_bit(byte, bit);
				bnum++;
			}
		}
	}
	*len = width;
}
