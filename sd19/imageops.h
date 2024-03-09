#pragma once
#ifndef IMAGEPOS_H_
#define IMAGEPOS_H_

unsigned char* cut_image(unsigned char* data, int sw, int sh, int* x, int y, int* w, int h);

unsigned char* cut_exact_image(unsigned char* data, int dw, int dh, int cx, int cy, int* cw, int ch);

unsigned char* imagedup(unsigned char* image, int width, int height, int depth);
#endif