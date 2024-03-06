#pragma once
#ifndef NORMALIZE_H_
#define NORMALIZE_H_

#include "mis.h"

#define NORM_W 32
#define NORM_H 32
#define RED_W  20
#define RED_H  32

#define UCHAR_HACK 1

void norm_2nd_gen(MIS** outmis, float** scal_x, float** scal_y, MIS* inmis, int* prisp_w, int* prisp_h);

void norm_2nd_gen2(MIS** outmis, float** scal_x, float** scal_y, MIS* inmis);
#endif