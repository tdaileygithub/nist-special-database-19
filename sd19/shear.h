#pragma once

#include "mis.h"

void shear_mis(MIS** smis, float** sfctr, MIS* imis);

float shear_char(unsigned char* idata, unsigned char* odata, int w, int  h);