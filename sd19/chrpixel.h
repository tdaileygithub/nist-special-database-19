#pragma once
#ifndef CHRPIXEL_H_
#define CHRPIXEL_H_

int pixelcnt(char* cdata, int w, int h);

void shift_row(unsigned char* irow, unsigned char* orow, int w, int sval);

int left_pix(unsigned char* row, int w);

int char_top(unsigned char* cdata, int w, int h);

int char_bottom(unsigned char* cdata, int w, int h);
#endif