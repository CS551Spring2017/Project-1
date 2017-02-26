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

int main(int argc, char *argv[])
{
	int i = 0;
	int count = 100;
	int delay = 1;
	char ch = '#';
	
	if (argc > 1)
	{
		count = atoi(argv[1]);
	}
	if (argc > 2)
	{
		ch = argv[2][0];
	}
	printf("Repeating char '%c' %d times...\n", ch, count);
	
	while (i < count)
	{
		printf("%c", ch);
		fflush(stdout);
		sleep(delay);
		i++;
	}
	
	printf("Done!\n");
	
	return 1;
}