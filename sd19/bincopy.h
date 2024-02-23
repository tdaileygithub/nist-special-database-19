#pragma once

#include "defs.h"

void binary_subimage_copy_8(
	u_char* src, int srcw, int srch,
	u_char* dst, int dstw, int dsth,
	int srcx, int srcy,
	int cpw, int cph,
	int dstx, int dsty
);

void binary_subimage_copy_gt(
	u_char* src, int srcw, int srch,
	u_char* dst, int dstw, int dsth,
	int srcx, int srcy,
	int cpw, int cph,
	int dstx, int dsty
);

void binary_subimage_copy_lt(
	u_char* src, int srcw, int srch,
	u_char* dst, int dstw, int dsth,
	int srcx, int srcy,
	int cpw, int cph,
	int dstx, int dsty
);

void binary_subimage_copy_eq(
	u_char* src, int srcw, int srch,
	u_char* dst, int dstw, int dsth,
	int srcx, int srcy,
	int cpw, int cph,
	int dstx, int dsty
);

void binary_subimage_copy(
	u_char* src, int srcw, int srch,
	u_char* dst, int dstw, int dsth,
	int srcx, int srcy,
	int cpw, int cph,
	int dstx, int dsty
);