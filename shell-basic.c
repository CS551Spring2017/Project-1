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
static char done = 0;				// for ctrl+c/ctrl+d handling
static void sigHandler(int signum)
{
	printf("\n"); // exit into new line
	done = 1;
}

int parseline();

int main()
{	
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sigHandler;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	
	while (!done)
	{
		printf(prompt);
		parseline();
	}
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
			printf("%s %s", prompt, buffer);
		}
		else if (c == '.') { // emergency exit
			system("/bin/stty cooked"); // enable input buffers
			exit(1);
		}
		else {
			printf("%c", c);
			buffer[i++] = c;
		}
	}
	system("/bin/stty cooked"); // enable input buffers
	
	size_t len = strlen(buffer);
	printf(" [I='%s'] [%d chars]\n", buffer, len);

	/*
	buffer = (char *)malloc(bufsize * sizeof(char));
	if (buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}
	
	characters = getline(&buffer, &bufsize, stdin);
	
	size_t len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n')
	{
		buffer[--len] = '\0';
	}
	printf("[I='%s'] [%d chars]\n", buffer, len);
	*/
	
	return 1;
}