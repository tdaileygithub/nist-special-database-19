#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include "syserr.h"

void syserr(const char* funcname, const char* syscall, const char* msg)
{
	int err;

	err = errno;

	(void)fflush(stdout);
	if (msg == NULL)
		(void)fprintf(stderr, "ERROR: %s: %s (%d", funcname, syscall, err);
	else
		(void)fprintf(stderr, "ERROR: %s: %s: %s (%d", funcname, syscall, msg, err);
	//if (err > 0 && err < sys_nerr)
	//	(void)fprintf(stderr, "; %s)\n", sys_errlist[err]);
	//else
	//	(void)fprintf(stderr, ")\n");
	(void)fflush(stderr);

	exit(-1);
}
