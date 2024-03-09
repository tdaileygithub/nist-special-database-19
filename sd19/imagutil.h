#pragma once
#ifndef IMAGUTIL_H_
#define IMAGUTIL_H_

float PixPerByte(int depth);
int SizeFromDepth(int pixwidth, int pixheight, int depth);
unsigned char* allocate_image(int width, int height, int depth);
#endif