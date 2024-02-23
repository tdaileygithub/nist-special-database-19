#pragma once

void erode_charimage(char* inp, char* out, int iw, int  ih);

void dilate_charimage(char* inp, char* out, int iw, int ih);

char get_south8(char* ptr, int row, int iw, int  ih);

char get_north8(char* ptr, int row, int iw);

char get_east8(char* ptr, int col, int iw);

char get_west8(char* ptr, int col);