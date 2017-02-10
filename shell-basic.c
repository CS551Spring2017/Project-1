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
#include <string.h>
#include <curses.h>

int parseline();

int main()
{
	initscr();
	timeout(-1);
	int c = getch();
	endwin();
	printf("%d %c\n", c, c);
	return 0;
	/*
	while (1)
	{
		printf("shell> ");
		parseline();
	}
	*/
}

int parseline()
{
	char *buffer;
	size_t bufsize = 32;
	size_t characters;

	buffer = (char *)malloc(bufsize * sizeof(char));
	if (buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}
	
	// test
	char c;
	while (1)
	{
		c = getchar();
		printf("Char: %c\n", c);
	}
	
	characters = getline(&buffer, &bufsize, stdin);
	
	size_t len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n')
	{
		buffer[--len] = '\0';
	}
	printf("[I='%s'] [%d chars]\n", buffer, len);
	
	return 1;
}