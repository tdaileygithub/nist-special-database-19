#pragma once

float PixPerByte(int depth);
int SizeFromDepth(int pixwidth, int pixheight, int depth);
unsigned char* allocate_image(int width, int height, int depth);