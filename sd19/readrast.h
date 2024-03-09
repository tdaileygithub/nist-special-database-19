#pragma once
#ifndef READRAST_H_
#define READRAST_H_

void ReadBinaryRaster(char* file, IHEAD** head, unsigned char** data, int* bpi, int* width, int* height);
#endif