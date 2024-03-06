#pragma once
#ifndef ZOOM_H_
#define ZOOM_H_

/*
# proc: zoom - scales a binary bitmap up or down depending on the x & y factors
# proc:        passed.
# proc: zoom_x - scales a binary bitmap up or down based on the x factor
# proc:          passed.
# proc: zoom_y - scales a binary bitmap up or down based on the y factor
# proc:          passed.
# proc: enlarge_x - expand a binary bitmap's columns by the x factor passed.
# proc:
# proc: enlarge_y - expand a binary bitmap's rows by the y factor passed.
# proc:
# proc: shrink_x - down sample a binary bitmap's columns by the x factor
# proc:            passed.
# proc: shrink_y - down sample a binary bitmap's rows by the y factor passed.
# proc:
# proc: copy_cols - copies a single column of pixels to a range of column
# proc:             locations in a destination image.
# proc: copy_col - copies a column of pixels to a column location in a
# proc:            destination image.
# proc: copy_rows - copies a single row of pixels to a range of row locations
# proc:             in a destination image.
# proc: copy_row - copies a row of pixels to a row location in a destination
# proc:            image.
*/

void zoom(unsigned char** zmdata, int* zw, int* zh, unsigned char* bindata, int iw, int  ih, float xfctr, float yfctr);

void zoom_x(char** xzmdata, int* zw, int* zh, char* chardata, int iw, int  ih, float xfctr);

void zoom_y(char** yzmdata, int* zw, int* zh, char* chardata, int iw, int ih, float yfctr);

void enlarge_x(char** xzmdata, int* zw, int* zh, char* chardata, int iw, int ih, float xfctr);

void enlarge_y(char** yzmdata, int* zw, int* zh, char* chardata, int  iw, int ih, float yfctr);

void shrink_x(char** xzmdata, int* zw, int* zh, char* chardata, int iw, int ih, float xfctr);

void shrink_y(char** yzmdata, int* zw, int* zh, char* chardata, int iw, int ih, float yfctr);

void copy_cols(char* xzmdata, int zw, int zh, int to1, int to2, char* chardata, int iw, int ih, int from);

void copy_col(char* xzmdata, int zw, int  zh, int  to, char* chardata, int iw, int ih, int  from);

void copy_rows(char* yzmdata, int zw, int  zh, int  to1, int  to2, char* chardata, int  iw, int  ih, int  from);

void copy_row(char* yzmdata, int zw, int zh, int to, char* chardata, int iw, int ih, int from);
#endif