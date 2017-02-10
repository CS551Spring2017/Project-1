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

static char *prompt = "shell> ";
static char done = 0;

int parseline();
int shell_proc(char *arg);
int verify_parenthesis_count(char *cmd);

int main()
{
	
	while (!done)
	{
		printf("%s", prompt);
		parseline();
	}
	
	return 0;
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
	
	int c;
	int i = 0;
	int ready = 0;
	system("/bin/stty raw"); // disable input buffers
	while (!ready)
	{
		c = getchar();
		if (c == '\r' || c == '\n') // 'enter' pressed
		{
			ready = 1;
			buffer[i++] = '\0';
		}
		else if (c == '\t') // 'tab' pressed
		{
			printf("\r\nSuggested: cmd1\r\n           cmd1 -l\r\n           cmd1 -l -h\r\n"); //sample
			printf("%s%s", prompt, buffer);
		}
		else if (c == '.') // emergency exit (temporary)
		{
			system("/bin/stty cooked"); // enable input buffers
			printf("\r\n");
			exit(1);
		}
		else if (c == '\b' || c == 8 || c == 127) // 'backspace' (8) or 'delete' (127)
		{
			if (i > 0)
			{
				printf("\b \b"); // replace char with a space
				i--;
			}
		}
		else {
			printf("%c", c);
			buffer[i++] = c;
		}
	}
	system("/bin/stty cooked"); // enable input buffers
	
	size_t len = strlen(buffer);
	printf(" [I='%s'] [%d chars]\n", buffer, len);
	
	shell_proc(buffer);
	
	return 1;
}

int shell_proc(char *arg)
{
	// do stuff
	// cmd1
	// cmd1;cmd2
	// cmd1&cmd2
	// (cmd1;cmd2)&(cmd1;cmd2)
	printf("pointer test: %s\n", arg);
	
	return 1;
}

int verify_parenthesis_count(char *cmd)
{
	// only allow '(' with matching ')'
}