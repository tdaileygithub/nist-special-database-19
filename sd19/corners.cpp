/*
# proc: find_corners - determines the box bounding the black pixels within
# proc:                a binary bitmap based on histograms and thresholds.
*/

#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <cstdlib>

#include "corners.h"
#include "defs.h"

#include "histgram.h"
#include "thres.h"

#define X_HIST 		0
#define Y_HIST		1

//#define BITLUT	"/usr/local/hsfsys/lut/bitcount.lut"
#define BITLUT		"bitcount.lut"
#define LUTLEN		256

/* find corners of region. returns top left x and y and object w and h */

void find_corners(int* tlx, int* tly, int* objw, int* objh, unsigned char* src_image, int width,int height, int thresh)
{
	int* xbins = NULL, num_x_bins = 0;
	int* ybins = NULL, num_y_bins = 0;
	int top = 0, topval = 0;
	int bottom = 0, bottomval = 0;
	int right = 0, rightval = 0;
	int left = 0, leftval = 0;

	compute_hist(src_image, width, height, X_HIST, &xbins, &num_x_bins);
	find_first_thresh_forward(xbins, 0, num_x_bins, thresh, &top, &topval);
	find_first_thresh_backward(xbins, 0, num_x_bins, thresh, &bottom, &bottomval);
	compute_hist(src_image, width, height, Y_HIST, &ybins, &num_y_bins);
	find_first_thresh_forward(ybins, 0, num_y_bins, thresh, &left, &leftval);
	find_first_thresh_backward(ybins, 0, num_y_bins, thresh, &right, &rightval);
	right = min(right + 1, num_y_bins);
	bottom = min(bottom + 1, num_x_bins);
	(*objw) = right - left;
	(*objh) = bottom - top;
	(*tlx) = left;
	(*tly) = top;
	free(xbins);
	free(ybins);
}
