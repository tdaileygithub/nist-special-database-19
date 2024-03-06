#pragma once
#ifndef WRITRAST_H_
#define WRITRAST_H_

#include "ihead.h"

void writeihdrfile(char* file, IHEAD* head, unsigned char* data);
#endif