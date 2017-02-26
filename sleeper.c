/*
 * CS551 - Project 1 - Shell
 * --
 * Team Members:
 *   John Paul Aldana
 *   Randal Kwok
 *   Sufyan Menk
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *msg = "World, hello!";

int main()
{
	printf("Printing '%s' in 10 seconds...\n", msg);
	sleep(10);
	printf("%s\n", msg);
	return 1;
}