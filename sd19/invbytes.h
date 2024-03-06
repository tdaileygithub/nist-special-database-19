#pragma once
#ifndef INVBYTES_H_
#define INVBYTES_H_

#define LUT_FILE "inv_byte.lut"
#define LUT_SIZE 512

void inv_bytes(unsigned char* data, int nbytes);
#endif