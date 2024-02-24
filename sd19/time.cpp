/*
# proc: current_time - returns a string containing the current date on the
# proc:                system.
*/

/************************************************************/
/*         Routines:   Current_Time()                       */
/*         Author:    Michael D. Garris                     */
/*         Date:      6/04/90                               */
/************************************************************/
#include <cassert>
#include <cstring>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "time.h"

/************************************************************/
/* Current_Time() gets the time of day from the system and  */
/* returns an ASCII date string without a newline char.     */
/************************************************************/
char* current_time()
{
	std::time_t result = std::time(nullptr);
	std::cout << std::ctime(&result);

	char buffer[32];
	std::strncpy(buffer, std::ctime(&result), 26);

	*(buffer + strlen(buffer) - 1) = '\0';
	return(buffer);
}
