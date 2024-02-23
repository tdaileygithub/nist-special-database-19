#pragma once

#define X_HIST 		0
#define Y_HIST		1

#define BITLUT		"bitcount.lut"
#define LUTLEN		256

void compute_hist(unsigned char* data, int width, int height,int orient, int** bins, int* len);

void compute_x_hist(unsigned char* data, int width, int height, int** bins, int* len);

void compute_y_hist(unsigned char* data, int width, int height, int** bins, int* len);
