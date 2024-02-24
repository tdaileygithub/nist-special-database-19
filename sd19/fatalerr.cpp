/*
# proc: fatalerr - generic application error handler that prints a specified
# proc:            message to stderr and exits with a status of 1.
*/

#include <cstdlib>
#include <stdio.h>

#include "fatalerr.h"

void fatalerr(const char* s1, const char* s2, const char* s3)
{

	(void)fflush(stdout);
	if (s2 == (char*)NULL)
		(void)fprintf(stderr, "ERROR: %s\n", s1);
	else if (s3 == (char*)NULL)
		(void)fprintf(stderr, "ERROR: %s: %s\n", s1, s2);
	else
		(void)fprintf(stderr, "ERROR: %s: %s: %s\n", s1, s2, s3);
	(void)fflush(stderr);

	exit(1);
}
