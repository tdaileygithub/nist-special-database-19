/*
# proc: bits2bytes - promotes a binary image to a grayscale image.
# proc:
*/

/* LINTLIBRARY */
#include <stdio.h>

#include "bits2bytes.h"
#include "defs.h"
#include "fatalerr.h"

void bits2bytes(u_char* p, u_char* q, unsigned int pixels)
{
	register double** rptrs, * dst;
	static u_char* ptrs[1 << BITSPERBYTE];
	static int init = 0;

	if (p == (u_char*)NULL)
		Fatal("bits2bytes: null src pointer");
	if (q == (u_char*)NULL)
		Fatal("bits2bytes: null dst pointer");

	if (!init) {
		int i, j;
		u_char* r;
		static u_char table[(1 << BITSPERBYTE) * BITSPERBYTE];
		static u_char mask[BITSPERBYTE] =
		{ 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

		init = 1;
		r = table;
		i = 0;
		while (i < (1 << BITSPERBYTE)) {
			ptrs[i] = r;
			for (j = 0; j < BITSPERBYTE; j++)
				*r++ = (i & mask[j]) ? 1 : 0;
			i++;
		}
	}


	dst = (double*)q;
	rptrs = (double**)ptrs;
	pixels = howmany(pixels, BITSPERBYTE);
	while (pixels--) {

#ifndef BCOPY_EXP
		* dst++ = *rptrs[*p++];
#else
		bcopy((char*)rptrs[*p++], (char*)dst++, BITSPERBYTE);
#endif

#ifdef DEBUG_EXP
		/* needs work */
		{
			int j;
			u_char* p2;

			(void)printf("%x -->", *p);
			p2 = rptrs[*p++];
			for (j = 0; j < BITSPERBYTE; j++)
				(void)printf(" %d", p2[j]);
			(void)printf("\n");
		}
#endif
	} /* WHILE */
}
