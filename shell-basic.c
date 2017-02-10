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

int parseline(char *buffer, size_t *bufsize);

int main()
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
	
	while (1)
	{
		printf("shell> ");
		parseline(buffer, bufsize);
	}
}

int parseline(char *buffer, size_t *bufsize)
{
	characters = getline(&buffer, &bufsize, stdin);
	printf("%zu characters were read.\n",characters);
	printf("You typed: '%s'\n",buffer);
}