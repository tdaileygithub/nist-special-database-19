#pragma once
#ifndef MISHDR_H_
#define MISHDR_H_

#include "ihead.h"

IHEAD* buildmishdr(char* misfile, int depth, int bpi, int misw, int mish, int entw, int enth, int ent_alloc, int compcode);

#endif