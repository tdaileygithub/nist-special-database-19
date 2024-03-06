#pragma once
#ifndef BINFILL_H_
#define BINFILL_H_

#include "defs.h"

void binary_fill_partial(int op, u_char* src, int i, u_char* dst, int j, int  bits);

int gb(u_char* p, int i);

void sb(u_char* p, int i, int v);

#endif