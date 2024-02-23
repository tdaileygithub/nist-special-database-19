/*
# proc: freemis - deallocates the memory associated with the specified
# proc:           mis structure.
# proc: freemishdr - deallocates the header associated with the specified
# proc:              mis structure. Only used after a call to readmishdr.
*/
#include <malloc.h>

#include "freemis.h"

void freemis(MIS* mis)
{
	free((char*)mis->head);
	free((char*)mis->data);
	free((char*)mis);
}

void freemishdr(MIS* mis)
{
	free((char*)mis->head);
	free((char*)mis);
}