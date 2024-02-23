#pragma once

/*********************************************************************/
/*          General Purpose Defines                                  */
/*********************************************************************/
#ifndef True
#define True		1
#define False		0
#endif

#ifndef TRUE
#define TRUE		True
#define FALSE		False
#endif

#define max(a, b)   ((a) > (b) ? (a) : (b))
#define min(a, b)   ((a) < (b) ? (a) : (b))
#define sround(x) ((int) (((x)<0) ? (x)-0.5 : (x)+0.5))

//http://tigcc.ticalc.org/doc/values.html
//values.h
#define BITSPERBYTE 8

typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

//https://cgit.freebsd.org/src/tree/sys/sys/param.h
#ifndef howmany
#define	howmany(x, y)	(((x)+((y)-1))/(y))
#endif
